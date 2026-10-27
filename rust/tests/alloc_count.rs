use mocopi_wupz_pq::{MocopiBoneTransform, MocopiFrame};
use std::alloc::{GlobalAlloc, Layout, System};
use std::sync::atomic::{AtomicUsize, Ordering};

struct Counting;

static ALLOCS: AtomicUsize = AtomicUsize::new(0);

unsafe impl GlobalAlloc for Counting {
    unsafe fn alloc(&self, layout: Layout) -> *mut u8 {
        ALLOCS.fetch_add(1, Ordering::Relaxed);
        unsafe { System.alloc(layout) }
    }

    unsafe fn dealloc(&self, ptr: *mut u8, layout: Layout) {
        unsafe { System.dealloc(ptr, layout) }
    }

    unsafe fn realloc(&self, ptr: *mut u8, layout: Layout, new_size: usize) -> *mut u8 {
        ALLOCS.fetch_add(1, Ordering::Relaxed);
        unsafe { System.realloc(ptr, layout, new_size) }
    }
}

#[global_allocator]
static A: Counting = Counting;

fn frame(n: u16) -> MocopiFrame {
    let mut bones = Vec::with_capacity(n as usize);
    for j in 0..n {
        bones.push(MocopiBoneTransform {
            joint_id: j,
            rotation: [0.0, 0.0, 0.0, 1.0],
            position: [0.1 * j as f32, 0.2 * j as f32, 0.3 * j as f32],
        });
    }
    MocopiFrame {
        sender_ip: 0x0100_007F,
        sender_port: 12351,
        frame_number: 1,
        timestamp: 1.0,
        utc_time: 1_761_080_000.0,
        timecode: [0, 0, 0, 0, 60, 0],
        bones,
    }
}

fn allocs_during<T>(f: impl FnOnce() -> T) -> (usize, T) {
    let before = ALLOCS.load(Ordering::Relaxed);
    let out = f();
    let after = ALLOCS.load(Ordering::Relaxed);
    (after - before, out)
}

#[test]
fn allocation_budget_for_encode_and_decode() {
    let f = frame(27);

    let (enc_allocs, encoded) = allocs_during(|| f.to_wupz_pq_bytes());
    assert_eq!(encoded.len(), 361);
    assert_eq!(encoded.capacity(), 361, "capacity must be exact, no slack");
    assert_eq!(enc_allocs, 1, "encode must allocate exactly once");

    let (dec_allocs, decoded) =
        allocs_during(|| MocopiFrame::from_wupz_pq_bytes(&encoded).expect("valid packet"));
    assert_eq!(decoded.bones.len(), 27);
    assert_eq!(
        dec_allocs, 1,
        "decode must allocate only the bones Vec (not 3 separate Vecs)"
    );
}
