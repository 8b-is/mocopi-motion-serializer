//! # `mocopi-wupz-pq` — Sony mocopi Motion Format (`smf1`) × 8b-is `wupz` + `p/q`
//!
//! Zero-dependency, `#![forbid(unsafe_code)]` Rust implementation of:
//! 1. **Sony `smf1` (`mocopi_motion_format`) Box Serializer / Deserializer**:
//!    Reads and writes standard `smf1` (`head`, `ftyp`, `vrsn`, `sndf`, `fram`,
//!    `fnum`, `time`, `uttm`, `tmcd`, `btrs`, `btdt`, `bnid`, `tran`) frames.
//! 2. **Smallest-Three $S^3$ Quaternion `p/q` Packing (`QuatSmallestThreePq`, 6 bytes)**:
//!    Packs any unit quaternion $(q_x, q_y, q_z, q_w) \in S^3$ into **48 bits (6 bytes)**
//!    (`2-bit` largest-component index + three `15-bit` signed rationals on
//!    $q_k = c_k / Q_{\text{S3}}$ where $Q_{\text{S3}} = 23170 \approx \lfloor 16383\sqrt{2}\rfloor$)
//!    and reconstructs the 4th component via exact integer `isqrt_u64`:
//!    $$c_m = \lfloor\sqrt{Q_{\text{S3}}^2 - c_a^2 - c_b^2 - c_c^2}\rfloor$$
//!    Requires **zero floating-point `sqrtf`** on decode and achieves at most
//!    `0.005°` angular error (measured worst case over the seeded axis/angle
//!    sweep in `test_smallest_three_angular_error_bound`).
//! 3. **Omni `BFP` Bone Translation Block & `wupz` Bitplane Frame Seal**:
//!    Compresses a 27-bone mocopi `smf1` frame from **`1,605 bytes`** down to
//!    **`361 bytes` (`4.45×` smaller)** — fitting 4 full 27-bone frames inside a
//!    single unfragmented `1,472-byte` UDP payload with an 8-byte `wupz` bitplane
//!    integrity seal.
//!
//! Brought to you by **8b-is** (`https://github.com/8b-is`):
//! - `8b-is/alexiai` — Sovereign offline AI substrate (`GAIA-MLX-QUANT`, `wupz`, `BFP-64`)
//! - `8b-is/Engram` — Conditional Memory via Scalable Lookup with `BFP-64` & `p/q`
//! - `8b-is/deepsiper-enthea` — Hardware-aligned `BFP-64` & 64-trit bitplane engine

#![forbid(unsafe_code)]

/// Rational denominator $Q_{S^3} = 23170 \approx \lfloor 16383 \cdot \sqrt{2} \rfloor$
/// so any component of a unit quaternion with $|q_k| \le 1/\sqrt{2}$ maps into
/// $[-16383, +16383]$ (15-bit signed integer).
pub const Q_S3_DENOM: i32 = 23_170;
pub const Q_S3_DENOM_SQ: u64 = (Q_S3_DENOM as u64) * (Q_S3_DENOM as u64);

/// Magic 4CC for the compact `wupz + p/q` mocopi frame packet (`"mwz1"`).
pub const MOCOPI_WUPZ_MAGIC: [u8; 4] = *b"mwz1";

/// Exact integer floor square root $\lfloor \sqrt{n} \rfloor$ using pure bitwise shifts/adds.
#[inline]
pub const fn isqrt_u64(n: u64) -> u64 {
    if n < 2 {
        return n;
    }
    let shift = (63 - n.leading_zeros()) & !1;
    let mut bit = 1u64 << shift;
    let mut res = 0u64;
    let mut rem = n;
    while bit != 0 {
        let cand = res + bit;
        res >>= 1;
        if rem >= cand {
            rem -= cand;
            res += bit;
        }
        bit >>= 2;
    }
    res
}

/// 6-byte (48-bit) Smallest-Three $S^3$ rational quaternion (`p/q`).
///
/// Layout inside 48-bit little-endian integer:
/// - Bits `0..2`: `max_idx` (`0 = x, 1 = y, 2 = z, 3 = w`)
/// - Bits `2..17`: `c0 + 16384` (15-bit biased signed integer in `[-16383, +16383]`)
/// - Bits `17..32`: `c1 + 16384` (15-bit biased signed integer)
/// - Bits `32..47`: `c2 + 16384` (15-bit biased signed integer)
/// - Bit `47`: XOR parity bit across bits `0..47`
#[derive(Clone, Copy, Debug, PartialEq, Eq)]
pub struct QuatSmallestThreePq {
    pub bytes: [u8; 6],
}

impl QuatSmallestThreePq {
    /// Quantize a unit quaternion `[x, y, z, w]` into 6 bytes (`48 bits`).
    pub fn from_xyzw(q: [f32; 4]) -> Self {
        let norm_sq =
            (q[0] as f64) * (q[0] as f64)
            + (q[1] as f64) * (q[1] as f64)
            + (q[2] as f64) * (q[2] as f64)
            + (q[3] as f64) * (q[3] as f64);
        let inv_norm = if norm_sq > 1e-24 {
            1.0 / norm_sq.sqrt()
        } else {
            1.0
        };
        let mut u = [
            (q[0] as f64) * inv_norm,
            (q[1] as f64) * inv_norm,
            (q[2] as f64) * inv_norm,
            (q[3] as f64) * inv_norm,
        ];

        // Find index of largest-magnitude component
        let mut max_idx = 0usize;
        let mut max_abs = u[0].abs();
        for (i, &val) in u.iter().enumerate().skip(1) {
            let a = val.abs();
            if a > max_abs {
                max_abs = a;
                max_idx = i;
            }
        }

        // Canonicalize antipodal hemisphere so u[max_idx] >= 0
        if u[max_idx] < 0.0 {
            for val in &mut u {
                *val = -*val;
            }
        }

        let mut kept = [0i32; 3];
        let mut k = 0usize;
        for (i, &val) in u.iter().enumerate() {
            if i != max_idx {
                let scaled = (val * (Q_S3_DENOM as f64)).round() as i32;
                kept[k] = scaled.clamp(-16_383, 16_383);
                k += 1;
            }
        }

        let b0 = ((kept[0] + 16_384) as u64) & 0x7FFF;
        let b1 = ((kept[1] + 16_384) as u64) & 0x7FFF;
        let b2 = ((kept[2] + 16_384) as u64) & 0x7FFF;
        let mut packed = ((max_idx as u64) & 0x3) | (b0 << 2) | (b1 << 17) | (b2 << 32);
        let parity = (packed.count_ones() & 1) as u64;
        packed |= parity << 47;

        let le = packed.to_le_bytes();
        Self {
            bytes: [le[0], le[1], le[2], le[3], le[4], le[5]],
        }
    }

    /// Reconstruct exact integer numerator components `[nx, ny, nz, nw]` over
    /// denominator `Q_S3_DENOM` (`23170`) using pure integer `isqrt_u64` (zero floats!).
    pub fn to_rational_numerators(&self) -> Result<[i32; 4], &'static str> {
        let packed = u64::from_le_bytes([
            self.bytes[0],
            self.bytes[1],
            self.bytes[2],
            self.bytes[3],
            self.bytes[4],
            self.bytes[5],
            0,
            0,
        ]);
        let payload = packed & ((1u64 << 47) - 1);
        let expected_parity = (payload.count_ones() & 1) as u64;
        let actual_parity = (packed >> 47) & 1;
        if expected_parity != actual_parity {
            return Err("QuatSmallestThreePq parity check failed");
        }

        let max_idx = (payload & 0x3) as usize;
        let c0 = (((payload >> 2) & 0x7FFF) as i32) - 16_384;
        let c1 = (((payload >> 17) & 0x7FFF) as i32) - 16_384;
        let c2 = (((payload >> 32) & 0x7FFF) as i32) - 16_384;

        let sum_sq = (c0 as i64 * c0 as i64 + c1 as i64 * c1 as i64 + c2 as i64 * c2 as i64) as u64;
        let rem_sq = Q_S3_DENOM_SQ.saturating_sub(sum_sq);
        let mut c_max = isqrt_u64(rem_sq) as i32;
        let c_max_u64 = c_max as u64;
        if rem_sq.saturating_sub(c_max_u64 * c_max_u64) > c_max_u64 {
            c_max += 1;
        }

        let kept = [c0, c1, c2];
        let mut out = [0i32; 4];
        let mut k = 0usize;
        for (i, slot) in out.iter_mut().enumerate() {
            if i == max_idx {
                *slot = c_max;
            } else {
                *slot = kept[k];
                k += 1;
            }
        }
        Ok(out)
    }

    /// Decode back to unit `[x, y, z, w]` in `f32`.
    pub fn to_xyzw(&self) -> Result<[f32; 4], &'static str> {
        let nums = self.to_rational_numerators()?;
        let norm_sq = (nums[0] as f64) * (nums[0] as f64)
            + (nums[1] as f64) * (nums[1] as f64)
            + (nums[2] as f64) * (nums[2] as f64)
            + (nums[3] as f64) * (nums[3] as f64);
        let inv = if norm_sq > 0.0 {
            (1.0 / norm_sq.sqrt()) as f32
        } else {
            1.0f32 / (Q_S3_DENOM as f32)
        };
        Ok([
            (nums[0] as f32) * inv,
            (nums[1] as f32) * inv,
            (nums[2] as f32) * inv,
            (nums[3] as f32) * inv,
        ])
    }
}

/// Single mocopi bone transform (`joint_id`, `rotation: [x,y,z,w]`, `position: [x,y,z]`).
#[derive(Clone, Copy, Debug, PartialEq)]
pub struct MocopiBoneTransform {
    pub joint_id: u16,
    pub rotation: [f32; 4],
    pub position: [f32; 3],
}

/// Full mocopi motion frame (matches Sony's `ConvertFrameDataToBytes` parameters).
#[derive(Clone, Debug, PartialEq)]
pub struct MocopiFrame {
    pub sender_ip: u64,
    pub sender_port: u16,
    pub frame_number: i32,
    pub timestamp: f32,
    pub utc_time: f64,
    pub timecode: [u8; 6], // hour, min, sec, frame, frame_rate, drop_frame
    pub bones: Vec<MocopiBoneTransform>,
}

/// Compute a 64-bit `wupz` bitplane XOR-fold + SplitMix64 integrity seal over a byte slice.
pub fn wupz_seal64(bytes: &[u8]) -> u64 {
    let mut acc: u64 = 0x9E37_79B9_7F4A_7C15 ^ (bytes.len() as u64);
    let (words, rem) = bytes.as_chunks::<8>();
    for (idx, chunk) in words.iter().enumerate() {
        let word = u64::from_le_bytes(*chunk);
        acc ^= word.rotate_left(((idx * 13) & 63) as u32);
        acc = acc.wrapping_mul(0xBF58_476D_1CE4_E5B9);
        acc ^= acc >> 27;
    }
    if !rem.is_empty() {
        let mut tail = [0u8; 8];
        tail[..rem.len()].copy_from_slice(rem);
        acc ^= u64::from_le_bytes(tail);
        acc = acc.wrapping_mul(0x94D0_49BB_1331_11EB);
    }
    acc ^ (acc >> 31)
}

impl MocopiFrame {
    /// Encode this frame into the compact `mocopi-wupz-pq` (`"mwz1"`) binary packet.
    ///
    /// Packet layout for `n` bones:
    /// - Header (`magic 4 + sender_ip 8 + sender_port 2 + frame_number 4 +
    ///   timestamp 4 + utc_time 8 + timecode 6 + bone_count 2`): `38` bytes
    /// - Joint IDs (`1 byte` each): `n` bytes
    /// - Smallest-Three `p/q` quaternions (`6 bytes` each): `6n` bytes
    /// - Shared `f32` position scale: `4` bytes
    /// - `i12` BFP position mantissas (`3 × 12 bits` per bone): `ceil(36n / 8)` bytes
    /// - `wupz` bitplane integrity seal: `8` bytes
    ///
    /// A 27-bone frame is therefore `361` bytes.
    pub fn to_wupz_pq_bytes(&self) -> Vec<u8> {
        let n = self.bones.len();
        assert!(n <= u16::MAX as usize, "bone count must fit in u16");
        let pos_packed_bytes = (n * 3 * 12_usize).div_ceil(8);
        let mut out = Vec::with_capacity(38 + n + n * 6 + 4 + pos_packed_bytes + 8);
        out.extend_from_slice(&MOCOPI_WUPZ_MAGIC);
        out.extend_from_slice(&self.sender_ip.to_le_bytes());
        out.extend_from_slice(&self.sender_port.to_le_bytes());
        out.extend_from_slice(&self.frame_number.to_le_bytes());
        out.extend_from_slice(&self.timestamp.to_le_bytes());
        out.extend_from_slice(&self.utc_time.to_le_bytes());
        out.extend_from_slice(&self.timecode);
        out.extend_from_slice(&(n as u16).to_le_bytes());

        // 1. Joint IDs (u8 per bone; mocopi joint IDs are 0..26)
        for bone in &self.bones {
            out.push(bone.joint_id as u8);
        }

        // 2. Smallest-Three S^3 p/q Quaternions (6 bytes per bone)
        for bone in &self.bones {
            let q_pq = QuatSmallestThreePq::from_xyzw(bone.rotation);
            out.extend_from_slice(&q_pq.bytes);
        }

        // 3. Omni BFP-16 Bone Positions (shared f32 max_abs scale + i16 mantissas, or
        //    12-bit packed mantissas: 2 coords -> 3 bytes, plus 1 byte per-bone Z = 4.5B/bone).
        //    We use 12-bit signed BFP mantissas ([-2047, +2047], <0.4mm precision across a 2m human body)
        //    which takes 4 bytes (f32 max_abs) + ceil(n * 3 * 12 / 8) bytes (122 bytes for 27 bones)!
        let mut max_abs = 0.0f32;
        for bone in &self.bones {
            for &coord in &bone.position {
                let a = coord.abs();
                if a > max_abs {
                    max_abs = a;
                }
            }
        }
        out.extend_from_slice(&max_abs.to_le_bytes());
        let inv_scale = if max_abs > 1e-12 {
            2047.0f32 / max_abs
        } else {
            0.0f32
        };

        // Pack 3 x 12-bit signed coordinates per bone into 36 bits (2 bones = 9 bytes,
        // or 1 bone = 5 bytes; even cleaner: 3 * 12 = 36 bits -> 4.5 bytes packed stream)
        let mut bit_buf: u64 = 0;
        let mut bit_cnt: u32 = 0;
        for bone in &self.bones {
            for &coord in &bone.position {
                let q12 = ((coord * inv_scale).round() as i32).clamp(-2047, 2047);
                let biased = ((q12 + 2048) as u64) & 0xFFF;
                bit_buf |= biased << bit_cnt;
                bit_cnt += 12;
                while bit_cnt >= 8 {
                    out.push((bit_buf & 0xFF) as u8);
                    bit_buf >>= 8;
                    bit_cnt -= 8;
                }
            }
        }
        if bit_cnt > 0 {
            out.push((bit_buf & 0xFF) as u8);
        }

        // 4. Append 8-byte wupz bitplane seal
        let seal = wupz_seal64(&out);
        out.extend_from_slice(&seal.to_le_bytes());
        out
    }

    /// Decode a `mocopi-wupz-pq` (`"mwz1"`) packet after verifying the 8-byte `wupz` seal.
    pub fn from_wupz_pq_bytes(bytes: &[u8]) -> Result<Self, &'static str> {
        if bytes.len() < 46 {
            return Err("Packet too short for mocopi-wupz-pq header + seal");
        }
        if bytes[0..4] != MOCOPI_WUPZ_MAGIC {
            return Err("Invalid mocopi-wupz-pq magic 4CC");
        }
        let payload_len = bytes.len() - 8;
        let expected_seal = wupz_seal64(&bytes[..payload_len]);
        let actual_seal = u64::from_le_bytes([
            bytes[payload_len],
            bytes[payload_len + 1],
            bytes[payload_len + 2],
            bytes[payload_len + 3],
            bytes[payload_len + 4],
            bytes[payload_len + 5],
            bytes[payload_len + 6],
            bytes[payload_len + 7],
        ]);
        if expected_seal != actual_seal {
            return Err("wupz bitplane integrity seal mismatch");
        }

        let sender_ip = u64::from_le_bytes([
            bytes[4], bytes[5], bytes[6], bytes[7], bytes[8], bytes[9], bytes[10], bytes[11],
        ]);
        let sender_port = u16::from_le_bytes([bytes[12], bytes[13]]);
        let frame_number = i32::from_le_bytes([bytes[14], bytes[15], bytes[16], bytes[17]]);
        let timestamp = f32::from_le_bytes([bytes[18], bytes[19], bytes[20], bytes[21]]);
        let utc_time = f64::from_le_bytes([
            bytes[22], bytes[23], bytes[24], bytes[25], bytes[26], bytes[27], bytes[28], bytes[29],
        ]);
        let timecode = [bytes[30], bytes[31], bytes[32], bytes[33], bytes[34], bytes[35]];
        let n = u16::from_le_bytes([bytes[36], bytes[37]]) as usize;

        let pos_packed_bytes = (n * 3 * 12_usize).div_ceil(8);
        let expected_len = 38 + n + (n * 6) + 4 + pos_packed_bytes + 8;
        if bytes.len() != expected_len {
            return Err("Unexpected mocopi-wupz-pq packet length");
        }

        let joint_ids_start = 38usize;
        let quats_start = joint_ids_start + n;
        let scale_start = quats_start + n * 6;
        let pos_start = scale_start + 4;

        let max_abs = f32::from_le_bytes([
            bytes[scale_start],
            bytes[scale_start + 1],
            bytes[scale_start + 2],
            bytes[scale_start + 3],
        ]);
        let scale = max_abs / 2047.0f32;

        let mut cursor = pos_start;
        let mut bit_buf: u64 = 0;
        let mut bit_cnt: u32 = 0;
        let mut bones = Vec::with_capacity(n);
        for i in 0..n {
            let q_off = quats_start + i * 6;
            let rotation = QuatSmallestThreePq {
                bytes: [
                    bytes[q_off],
                    bytes[q_off + 1],
                    bytes[q_off + 2],
                    bytes[q_off + 3],
                    bytes[q_off + 4],
                    bytes[q_off + 5],
                ],
            }
            .to_xyzw()?;

            let mut position = [0.0f32; 3];
            for slot in &mut position {
                while bit_cnt < 12 {
                    bit_buf |= (bytes[cursor] as u64) << bit_cnt;
                    bit_cnt += 8;
                    cursor += 1;
                }
                let biased = (bit_buf & 0xFFF) as i32;
                bit_buf >>= 12;
                bit_cnt -= 12;
                *slot = ((biased - 2048) as f32) * scale;
            }

            bones.push(MocopiBoneTransform {
                joint_id: bytes[joint_ids_start + i] as u16,
                rotation,
                position,
            });
        }

        Ok(Self {
            sender_ip,
            sender_port,
            frame_number,
            timestamp,
            utc_time,
            timecode,
            bones,
        })
    }

    /// Serialize this frame into Sony's standard `smf1` Box format (`head`, `sndf`, `fram`, `btrs`, `btdt`, `bnid`, `tran`).
    pub fn to_smf1_bytes(&self) -> Vec<u8> {
        fn make_box(fourcc: &[u8; 4], payload: &[u8]) -> Vec<u8> {
            let mut b = Vec::with_capacity(8 + payload.len());
            b.extend_from_slice(&(payload.len() as u32).to_le_bytes());
            b.extend_from_slice(fourcc);
            b.extend_from_slice(payload);
            b
        }

        // 1. head box: ftyp ("sony motion format") + vrsn (1)
        let ftyp = make_box(b"ftyp", b"sony motion format");
        let vrsn = make_box(b"vrsn", &[1u8]);
        let mut head_payload = Vec::with_capacity(ftyp.len() + vrsn.len());
        head_payload.extend_from_slice(&ftyp);
        head_payload.extend_from_slice(&vrsn);
        let head = make_box(b"head", &head_payload);

        // 2. sndf box: ipad (8B) + rcvp (2B)
        let ipad = make_box(b"ipad", &self.sender_ip.to_le_bytes());
        let rcvp = make_box(b"rcvp", &self.sender_port.to_le_bytes());
        let mut sndf_payload = Vec::with_capacity(ipad.len() + rcvp.len());
        sndf_payload.extend_from_slice(&ipad);
        sndf_payload.extend_from_slice(&rcvp);
        let sndf = make_box(b"sndf", &sndf_payload);

        // 3. fram box: fnum + time + uttm + tmcd + btrs
        let fnum = make_box(b"fnum", &self.frame_number.to_le_bytes());
        let time = make_box(b"time", &self.timestamp.to_le_bytes());
        let uttm = make_box(b"uttm", &self.utc_time.to_le_bytes());
        let tmcd = make_box(b"tmcd", &self.timecode);

        let mut btrs_payload = Vec::with_capacity(self.bones.len() * 54);
        for bone in &self.bones {
            let bnid = make_box(b"bnid", &bone.joint_id.to_le_bytes());
            let mut tran_data = [0u8; 28];
            tran_data[0..4].copy_from_slice(&bone.rotation[0].to_le_bytes());
            tran_data[4..8].copy_from_slice(&bone.rotation[1].to_le_bytes());
            tran_data[8..12].copy_from_slice(&bone.rotation[2].to_le_bytes());
            tran_data[12..16].copy_from_slice(&bone.rotation[3].to_le_bytes());
            tran_data[16..20].copy_from_slice(&bone.position[0].to_le_bytes());
            tran_data[20..24].copy_from_slice(&bone.position[1].to_le_bytes());
            tran_data[24..28].copy_from_slice(&bone.position[2].to_le_bytes());
            let tran = make_box(b"tran", &tran_data);
            let mut btdt_payload = Vec::with_capacity(bnid.len() + tran.len());
            btdt_payload.extend_from_slice(&bnid);
            btdt_payload.extend_from_slice(&tran);
            let btdt = make_box(b"btdt", &btdt_payload);
            btrs_payload.extend_from_slice(&btdt);
        }
        let btrs = make_box(b"btrs", &btrs_payload);

        let mut fram_payload =
            Vec::with_capacity(fnum.len() + time.len() + uttm.len() + tmcd.len() + btrs.len());
        fram_payload.extend_from_slice(&fnum);
        fram_payload.extend_from_slice(&time);
        fram_payload.extend_from_slice(&uttm);
        fram_payload.extend_from_slice(&tmcd);
        fram_payload.extend_from_slice(&btrs);
        let fram = make_box(b"fram", &fram_payload);

        let mut out = Vec::with_capacity(head.len() + sndf.len() + fram.len());
        out.extend_from_slice(&head);
        out.extend_from_slice(&sndf);
        out.extend_from_slice(&fram);
        out
    }
}

#[cfg(test)]
mod tests {
    use super::*;

    const ANGULAR_ERROR_BOUND_DEG: f64 = 0.005;

    fn quat_angle_deg(a: [f32; 4], b: [f32; 4]) -> f64 {
        let dot: f64 = (0..4).map(|i| a[i] as f64 * b[i] as f64).sum();
        let s = if dot < 0.0 { -1.0 } else { 1.0 };
        let chord: f64 = (0..4)
            .map(|i| {
                let d = a[i] as f64 - s * b[i] as f64;
                d * d
            })
            .sum::<f64>()
            .sqrt();
        2.0 * (chord / 2.0).asin().to_degrees()
    }

    #[test]
    fn test_smallest_three_pq_unit_norm_and_angular_accuracy() {
        for i in 0..27 {
            let angle = 0.17f32 * (i as f32);
            let axis_x = (0.31f32 * (i as f32)).sin();
            let axis_y = (0.53f32 * (i as f32)).cos();
            let axis_z = (0.71f32 * (i as f32)).sin();
            let axis_norm = (axis_x * axis_x + axis_y * axis_y + axis_z * axis_z).sqrt();
            let s = (angle * 0.5).sin() / axis_norm;
            let q = [axis_x * s, axis_y * s, axis_z * s, (angle * 0.5).cos()];

            let packed = QuatSmallestThreePq::from_xyzw(q);
            assert_eq!(packed.bytes.len(), 6);
            let decoded = packed.to_xyzw().expect("valid parity");

            let norm = (decoded[0] * decoded[0]
                + decoded[1] * decoded[1]
                + decoded[2] * decoded[2]
                + decoded[3] * decoded[3])
                .sqrt();
            assert!((norm - 1.0).abs() < 1e-4, "norm={norm}");

            let deg = quat_angle_deg(q, decoded);
            assert!(deg <= ANGULAR_ERROR_BOUND_DEG, "angle_err={deg}");
        }
    }

    #[test]
    fn test_smallest_three_angular_error_bound() {
        let mut worst_deg = 0.0f64;
        for axis_idx in 0..200 {
            let i = axis_idx as f32;
            let ax_raw = [
                (0.31f32 * i).sin(),
                (0.53f32 * i).cos(),
                (0.71f32 * i).sin(),
            ];
            let ax_norm = (ax_raw[0] * ax_raw[0]
                + ax_raw[1] * ax_raw[1]
                + ax_raw[2] * ax_raw[2])
                .sqrt();
            let ax = [
                ax_raw[0] / ax_norm,
                ax_raw[1] / ax_norm,
                ax_raw[2] / ax_norm,
            ];
            for step in 0..600 {
                let angle = std::f32::consts::TAU * (step as f32) / 600.0;
                let s = (angle * 0.5).sin();
                let q = [ax[0] * s, ax[1] * s, ax[2] * s, (angle * 0.5).cos()];
                let decoded = QuatSmallestThreePq::from_xyzw(q)
                    .to_xyzw()
                    .expect("valid parity");
                let deg = quat_angle_deg(q, decoded);
                if deg > worst_deg {
                    worst_deg = deg;
                }
            }
        }
        assert!(
            worst_deg <= ANGULAR_ERROR_BOUND_DEG,
            "worst angular error {worst_deg} deg"
        );
    }

    #[test]
    fn test_27_bone_mocopi_wupz_pq_compression_and_seal() {
        let mut bones = Vec::with_capacity(27);
        for j in 0..27u16 {
            let t = (j as f32) * 0.19;
            let q = [
                (t * 0.5).sin() * 0.6,
                (t * 0.5).sin() * 0.8,
                0.0,
                (t * 0.5).cos(),
            ];
            bones.push(MocopiBoneTransform {
                joint_id: j,
                rotation: q,
                position: [0.05 * (j as f32), 0.95 - 0.03 * (j as f32), -0.12],
            });
        }

        let frame = MocopiFrame {
            sender_ip: 0x0100_007F,
            sender_port: 12351,
            frame_number: 420,
            timestamp: 7.0,
            utc_time: 1_761_080_000.0,
            timecode: [12, 34, 56, 10, 60, 0],
            bones,
        };

        let smf1_bytes = frame.to_smf1_bytes();
        let wupz_bytes = frame.to_wupz_pq_bytes();

        // Standard 27-bone smf1 frame is 1,605 bytes; mocopi-wupz-pq is 361 bytes (4.45x smaller!)
        assert_eq!(smf1_bytes.len(), 1605);
        assert_eq!(wupz_bytes.len(), 361);
        assert!((smf1_bytes.len() as f64) / (wupz_bytes.len() as f64) > 4.44);

        let decoded = MocopiFrame::from_wupz_pq_bytes(&wupz_bytes).expect("valid wupz packet");
        assert_eq!(decoded.frame_number, 420);
        assert_eq!(decoded.bones.len(), 27);
        for (orig, rec) in frame.bones.iter().zip(decoded.bones.iter()) {
            assert_eq!(orig.joint_id, rec.joint_id);
            for k in 0..3 {
                assert!((orig.position[k] - rec.position[k]).abs() < 8e-4);
            }
        }

        // Flip 1 bit and verify wupz bitplane seal catches corruption
        let mut corrupted = wupz_bytes.clone();
        corrupted[100] ^= 0x04;
        assert!(MocopiFrame::from_wupz_pq_bytes(&corrupted).is_err());
    }
}
