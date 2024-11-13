// Ignore deprecation warnings inside our own runtime
#define HALIDE_ALLOW_DEPRECATED 1

#include "HalideRuntime.h"
#include "HalideRuntimeCuda.h"
#include "HalideRuntimeD3D12Compute.h"
#include "HalideRuntimeHexagonHost.h"
#include "HalideRuntimeMetal.h"
#include "HalideRuntimeOpenCL.h"
#include "HalideRuntimeQurt.h"
#include "HalideRuntimeVulkan.h"
#include "HalideRuntimeWebGPU.h"
#include "cpu_features.h"

// This runtime module will contain extern declarations of the Halide
// API and the types it uses. It's useful for compiling modules that
// use the API without including a copy of it (e.g. JIT, NoRuntime).

// Can be generated via the following:
// cat src/runtime/runtime_internal.h src/runtime/HalideRuntime*.h | grep "^[^ ][^(]*halide_[^ ]*(" | grep -v '#define' | sed "s/[^(]*halide/halide/" | sed "s/(.*//" | sed "s/^h/    \(void *)\&h/" | sed "s/$/,/" | sort | uniq

extern "C" void halide_unused_force_include_types();

extern "C" __attribute__((used)) void *halide_runtime_api_functions[] = {
    (void *)&halide_buffer_copy,
    (void *)&halide_buffer_to_string,
    (void *)&halide_can_use_target_features,
    (void *)&halide_cond_broadcast,
    (void *)&halide_cond_signal,
    (void *)&halide_cond_wait,
    (void *)&halide_copy_to_device,
    (void *)&halide_copy_to_host,
    (void *)&halide_cuda_detach_device_ptr,
    (void *)&halide_cuda_device_interface,
    (void *)&halide_cuda_get_device_ptr,
    (void *)&halide_cuda_initialize_kernels,
    (void *)&halide_cuda_finalize_kernels,
    (void *)&halide_cuda_run,
    (void *)&halide_cuda_wrap_device_ptr,
    (void *)&halide_current_time_ns,
    (void *)&halide_debug_to_file,
    (void *)&halide_default_can_use_target_features,
    (void *)&halide_device_and_host_free,
    (void *)&halide_device_and_host_free_as_destructor,
    (void *)&halide_device_and_host_malloc,
    (void *)&halide_device_free,
    (void *)&halide_device_free_as_destructor,
    (void *)&halide_device_host_nop_free,
    (void *)&halide_device_malloc,
    (void *)&halide_device_release,
    (void *)&halide_device_sync,
    (void *)&halide_device_sync_global,
    (void *)&halide_disable_timer_interrupt,
    (void *)&halide_do_par_for,
    (void *)&halide_do_parallel_tasks,
    (void *)&halide_do_task,
    (void *)&halide_do_loop_task,
    (void *)&halide_double_to_string,
    (void *)&halide_enable_timer_interrupt,
    (void *)&halide_error,
    (void *)&halide_error_access_out_of_bounds,
    (void *)&halide_error_bad_dimensions,
    (void *)&halide_error_bad_fold,
    (void *)&halide_error_bad_extern_fold,
    (void *)&halide_error_bad_type,
    (void *)&halide_error_bounds_inference_call_failed,
    (void *)&halide_error_buffer_allocation_too_large,
    (void *)&halide_error_buffer_argument_is_null,
    (void *)&halide_error_buffer_extents_negative,
    (void *)&halide_error_buffer_extents_too_large,
    (void *)&halide_error_constraint_violated,
    (void *)&halide_error_constraints_make_required_region_smaller,
    (void *)&halide_error_debug_to_file_failed,
    (void *)&halide_error_device_dirty_with_no_device_support,
    (void *)&halide_error_explicit_bounds_too_small,
    (void *)&halide_error_extern_stage_failed,
    (void *)&halide_error_fold_factor_too_small,
    (void *)&halide_error_host_is_null,
    (void *)&halide_error_out_of_memory,
    (void *)&halide_error_param_too_large_f64,
    (void *)&halide_error_param_too_large_i64,
    (void *)&halide_error_param_too_large_u64,
    (void *)&halide_error_param_too_small_f64,
    (void *)&halide_error_param_too_small_i64,
    (void *)&halide_error_param_too_small_u64,
    (void *)&halide_error_requirement_failed,
    (void *)&halide_error_specialize_fail,
    (void *)&halide_error_split_factor_not_positive,
    (void *)&halide_error_unaligned_host_ptr,
    (void *)&halide_error_storage_bound_too_small,
    (void *)&halide_error_device_crop_failed,
    (void *)&halide_error_vscale_invalid,
    (void *)&halide_float16_bits_to_double,
    (void *)&halide_float16_bits_to_float,
    (void *)&halide_free,
    (void *)&halide_get_cpu_features,
    (void *)&halide_get_gpu_device,
    (void *)&halide_get_library_symbol,
    (void *)&halide_get_num_threads,
    (void *)&halide_get_symbol,
    (void *)&halide_get_trace_file,
    (void *)&halide_hexagon_detach_device_handle,
    (void *)&halide_hexagon_device_interface,
    (void *)&halide_hexagon_device_release,
    (void *)&halide_hexagon_get_device_handle,
    (void *)&halide_hexagon_get_device_size,
    (void *)&halide_hexagon_get_module_state,
    (void *)&halide_hexagon_initialize_kernels,
    (void *)&halide_hexagon_finalize_kernels,
    (void *)&halide_hexagon_power_hvx_off,
    (void *)&halide_hexagon_power_hvx_off_as_destructor,
    (void *)&halide_hexagon_power_hvx_on,
    (void *)&halide_hexagon_run,
    (void *)&halide_hexagon_set_performance,
    (void *)&halide_hexagon_set_performance_mode,
    (void *)&halide_hexagon_set_thread_priority,
    (void *)&halide_hexagon_wrap_device_handle,
    (void *)&halide_int64_to_string,
    (void *)&halide_join_thread,
    (void *)&halide_load_library,
    (void *)&halide_malloc,
    (void *)&halide_memoization_cache_cleanup,
    (void *)&halide_memoization_cache_evict,
    (void *)&halide_memoization_cache_lookup,
    (void *)&halide_memoization_cache_release,
    (void *)&halide_memoization_cache_set_size,
    (void *)&halide_memoization_cache_store,
    (void *)&halide_metal_acquire_context,
    (void *)&halide_metal_detach_buffer,
    (void *)&halide_metal_device_interface,
    (void *)&halide_metal_get_buffer,
    (void *)&halide_metal_get_crop_offset,
    (void *)&halide_metal_initialize_kernels,
    (void *)&halide_metal_finalize_kernels,
    (void *)&halide_metal_release_context,
    (void *)&halide_metal_run,
    (void *)&halide_metal_wrap_buffer,
    (void *)&halide_msan_annotate_buffer_is_initialized,
    (void *)&halide_msan_annotate_buffer_is_initialized_as_destructor,
    (void *)&halide_msan_annotate_memory_is_initialized,
    (void *)&halide_msan_check_buffer_is_initialized,
    (void *)&halide_msan_check_memory_is_initialized,
    (void *)&halide_mutex_lock,
    (void *)&halide_mutex_unlock,
    (void *)&halide_mutex_array_create,
    (void *)&halide_mutex_array_destroy,
    (void *)&halide_mutex_array_lock,
    (void *)&halide_mutex_array_unlock,
    (void *)&halide_opencl_detach_cl_mem,
    (void *)&halide_opencl_device_interface,
    (void *)&halide_opencl_get_cl_mem,
    (void *)&halide_opencl_get_build_options,
    (void *)&halide_opencl_get_device_type,
    (void *)&halide_opencl_get_platform_name,
    (void *)&halide_opencl_get_crop_offset,
    (void *)&halide_opencl_image_device_interface,
    (void *)&halide_opencl_image_wrap_cl_mem,
    (void *)&halide_opencl_initialize_kernels,
    (void *)&halide_opencl_finalize_kernels,
    (void *)&halide_opencl_run,
    (void *)&halide_opencl_set_build_options,
    (void *)&halide_opencl_set_device_type,
    (void *)&halide_opencl_set_platform_name,
    (void *)&halide_opencl_wrap_cl_mem,
    (void *)&halide_pointer_to_string,
    (void *)&halide_print,
    (void *)&halide_profiler_get_pipeline_state,
    (void *)&halide_profiler_get_state,
    (void *)&halide_profiler_instance_start,
    (void *)&halide_profiler_instance_end,
    (void *)&halide_profiler_memory_allocate,
    (void *)&halide_profiler_memory_free,
    (void *)&halide_profiler_report,
    (void *)&halide_profiler_reset,
    (void *)&halide_profiler_stack_peak_update,
    (void *)&halide_qurt_hvx_lock,
    (void *)&halide_qurt_hvx_unlock,
    (void *)&halide_qurt_hvx_unlock_as_destructor,
    (void *)&halide_release_jit_module,
    (void *)&halide_semaphore_init,
    (void *)&halide_semaphore_release,
    (void *)&halide_semaphore_try_acquire,
    (void *)&halide_set_custom_can_use_target_features,
    (void *)&halide_set_custom_do_par_for,
    (void *)&halide_set_custom_do_loop_task,
    (void *)&halide_set_custom_do_task,
    (void *)&halide_set_custom_free,
    (void *)&halide_set_custom_get_library_symbol,
    (void *)&halide_set_custom_get_symbol,
    (void *)&halide_set_custom_load_library,
    (void *)&halide_set_custom_malloc,
    (void *)&halide_set_custom_print,
    (void *)&halide_set_custom_trace,
    (void *)&halide_set_error_handler,
    (void *)&halide_set_gpu_device,
    (void *)&halide_set_num_threads,
    (void *)&halide_set_trace_file,
    (void *)&halide_shutdown_thread_pool,
    (void *)&halide_shutdown_trace,
    (void *)&halide_sleep_us,
    (void *)&halide_spawn_thread,
    (void *)&halide_start_clock,
    (void *)&halide_start_timer_chain,
    (void *)&halide_string_to_string,
    (void *)&halide_trace,
    (void *)&halide_trace_helper,
    (void *)&halide_uint64_to_string,
    (void *)&halide_use_jit_module,
    (void *)&halide_d3d12compute_acquire_context,
    (void *)&halide_d3d12compute_device_interface,
    (void *)&halide_d3d12compute_initialize_kernels,
    (void *)&halide_d3d12compute_finalize_kernels,
    (void *)&halide_d3d12compute_release_context,
    (void *)&halide_d3d12compute_run,
    (void *)&halide_vulkan_acquire_context,
    (void *)&halide_vulkan_device_interface,
    (void *)&halide_vulkan_initialize_kernels,
    (void *)&halide_vulkan_release_context,
    (void *)&halide_vulkan_run,
    (void *)&halide_webgpu_device_interface,
    (void *)&halide_webgpu_initialize_kernels,
    (void *)&halide_webgpu_finalize_kernels,
    (void *)&halide_webgpu_run,
    (void *)&halide_unused_force_include_types,
};
