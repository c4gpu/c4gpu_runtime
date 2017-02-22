//#include "c4g_compiler.h"
#include "c4g_runtime.h"
#include <stdio.h>
#include <Windows.h>

//#define COMPILER_LIB_NAME L"c4gcp.dll"
#define RUNTIME_LIB_NAME L"c4grt.dll"

#ifndef C4G_COUNTOF
#	define C4G_COUNTOF(__a) (sizeof(__a) / sizeof(*(__a)))
#endif /* C4G_COUNTOF */

/*
struct Compiler final {
	HINSTANCE _hDll = nullptr;

	decltype(&c4gcp_open) open = nullptr;
	decltype(&c4gcp_close) close = nullptr;

	Compiler::Compiler() {
	}
	Compiler::~Compiler() {
	}

	bool load(void) {
		_hDll = LoadLibrary(COMPILER_LIB_NAME);

		if (!_hDll) {
			wprintf(L"Cannot find library %s.\n", COMPILER_LIB_NAME);

			return false;
		}

		open = (decltype(open))GetProcAddress(_hDll, "c4gcp_open");
		close = (decltype(close))GetProcAddress(_hDll, "c4gcp_close");

		bool ret = (
			!!open ||
			!!close
		);
		if (!ret) {
			wprintf(L"Incomplete library %s.\n", COMPILER_LIB_NAME);

			return false;
		}

		return true;
	}
	bool unload(void) {
		if (!_hDll)
			return false;

		FreeLibrary(_hDll);

		return true;
	}
};
*/

struct Runtime final {
	HINSTANCE _hDll = nullptr;

	decltype(&c4grt_open) open = nullptr;
	decltype(&c4grt_close) close = nullptr;

	decltype(&c4grt_show_driver_info) showDriverInfo = nullptr;

	decltype(&c4grt_begin) begin = nullptr;
	decltype(&c4grt_end) end = nullptr;

	decltype(&c4grt_add_pass) addPass = nullptr;
	decltype(&c4grt_set_pass_flow) setPassFlow = nullptr;
	decltype(&c4grt_set_pass_pipe) setPassPipe = nullptr;

#ifndef C4GRT_WITHOUT_EMITTER
	decltype(&c4grt_use_pcode_file) usePcodeFile = nullptr;
	decltype(&c4grt_use_pcode_string) usePcodeString = nullptr;
#endif /* !C4GRT_WITHOUT_EMITTER */

	decltype(&c4grt_use_gpu_program_file) useGpuProgramFile = nullptr;
	decltype(&c4grt_use_gpu_program_string) useGpuProgramString = nullptr;

	decltype(&c4grt_prepare_buffers) prepareBuffers = nullptr;
	decltype(&c4grt_prepare_tex) prepareTex = nullptr;
	decltype(&c4grt_prepare_uniform) prepareUniform = nullptr;
	decltype(&c4grt_prepare_in) prepareIn = nullptr;
	decltype(&c4grt_prepare_out) prepareOut = nullptr;
	decltype(&c4grt_compute) compute = nullptr;
	decltype(&c4grt_map_out) mapOut = nullptr;
	decltype(&c4grt_finish) finish = nullptr;

	Runtime::Runtime() {
	}
	Runtime::~Runtime() {
	}

	bool load(void) {
		_hDll = LoadLibrary(RUNTIME_LIB_NAME);

		if (!_hDll) {
			wprintf(L"Cannot find library %s.\n", RUNTIME_LIB_NAME);

			return false;
		}

		open = (decltype(open))GetProcAddress(_hDll, "c4grt_open");
		close = (decltype(close))GetProcAddress(_hDll, "c4grt_close");
		showDriverInfo = (decltype(showDriverInfo))GetProcAddress(_hDll, "c4grt_show_driver_info");
		begin = (decltype(begin))GetProcAddress(_hDll, "c4grt_begin");
		end = (decltype(end))GetProcAddress(_hDll, "c4grt_end");
		addPass = (decltype(addPass))GetProcAddress(_hDll, "c4grt_add_pass");
		setPassFlow = (decltype(setPassFlow))GetProcAddress(_hDll, "c4grt_set_pass_flow");
		setPassPipe = (decltype(setPassPipe))GetProcAddress(_hDll, "c4grt_set_pass_pipe");
#ifndef C4GRT_WITHOUT_EMITTER
		usePcodeFile = (decltype(usePcodeFile))GetProcAddress(_hDll, "c4grt_use_pcode_file");
		usePcodeString = (decltype(usePcodeString))GetProcAddress(_hDll, "c4grt_use_pcode_string");
#endif /* !C4GRT_WITHOUT_EMITTER */
		useGpuProgramFile = (decltype(useGpuProgramFile))GetProcAddress(_hDll, "c4grt_use_gpu_program_file");
		useGpuProgramString = (decltype(useGpuProgramString))GetProcAddress(_hDll, "c4grt_use_gpu_program_string");
		prepareBuffers = (decltype(prepareBuffers))GetProcAddress(_hDll, "c4grt_prepare_buffers");
		prepareTex = (decltype(prepareTex))GetProcAddress(_hDll, "c4grt_prepare_tex");
		prepareUniform = (decltype(prepareUniform))GetProcAddress(_hDll, "c4grt_prepare_uniform");
		prepareIn = (decltype(prepareIn))GetProcAddress(_hDll, "c4grt_prepare_in");
		prepareOut = (decltype(prepareOut))GetProcAddress(_hDll, "c4grt_prepare_out");
		compute = (decltype(compute))GetProcAddress(_hDll, "c4grt_compute");
		mapOut = (decltype(mapOut))GetProcAddress(_hDll, "c4grt_map_out");
		finish = (decltype(finish))GetProcAddress(_hDll, "c4grt_finish");

		bool ret = (
			!!open ||
			!!close ||
			!!showDriverInfo ||
			!!begin ||
			!!end ||
			!!addPass ||
			!!setPassFlow ||
			!!setPassPipe ||
#ifndef C4GRT_WITHOUT_EMITTER
			!!usePcodeFile ||
			!!usePcodeString ||
#endif /* !C4GRT_WITHOUT_EMITTER */
			!!useGpuProgramFile ||
			!!useGpuProgramString ||
			!!prepareBuffers ||
			!!prepareTex ||
			!!prepareUniform ||
			!!prepareIn ||
			!!prepareOut ||
			!!compute ||
			!!mapOut ||
			!!finish
		);
		if (!ret) {
			wprintf(L"Incomplete library %s.\n", RUNTIME_LIB_NAME);

			return false;
		}

		return true;
	}
	bool unload(void) {
		if (!_hDll)
			return false;

		FreeLibrary(_hDll);

		return true;
	}
};

int main(int argc, char* argv[]) {
	printf("C4G\n");

	Runtime r;
	if (!r.load()) return -1;

	struct C4GRT_Runtime* rt = r.open();

	r.showDriverInfo(rt);

	r.begin(rt);
	{
		const C4GRT_Vec4b ptx0[] = { { 64, 64, 64, 64 }, { 64, 64, 64, 64 }, { 64, 64, 64, 64 }, { 64, 64, 64, 64 } };
		const C4GRT_Tex ptx[] = {
			{ DT_VEC4B, (unsigned char*)ptx0, C4G_COUNTOF(ptx0), sizeof(C4GRT_Vec4b), "s0", { 2, 2 }, 2 }
		};
		const C4GRT_Vec4b tx0[] = { { 128, 128, 128, 128 }, { 128, 128, 128, 128 }, { 128, 128, 128, 128 }, { 128, 128, 128, 128 } };
		const C4GRT_Tex tx[] = {
			{ DT_VEC4B, (unsigned char*)tx0, C4G_COUNTOF(tx0), sizeof(C4GRT_Vec4b), "s0", { 2, 2 }, 2 }
		};
		const C4GRT_Vec4 un0 = { 100, 100, 100, 100 };
		const C4GRT_Data un[] = {
			{ DT_VEC4, (unsigned char*)&un0, 1, sizeof(C4GRT_Vec4), "u0" }
		};
		const C4GRT_Vec4 in0[] = { { 1.3f, 1.4f, 2.1f, 0.123f }, { 2.2f, 1.5f, 0.001f, 0.123f }, { 1.6f, 0.02f, 1.1f, 0.123f }, { 0.05f, 0.03f, 1.2f, 0.123f } };
		const C4GRT_Vec4 in1[] = { { 10, 20, 30, 40 }, { 5, 6, 7, 8 }, { 9, 10, 11, 12 }, { 13, 14, 15, 16 } };
		const C4GRT_Data in[] = {
			{ DT_VEC4, (unsigned char*)in0, C4G_COUNTOF(in0), sizeof(C4GRT_Vec4), "v0" },
			{ DT_VEC4, (unsigned char*)in1, C4G_COUNTOF(in1), sizeof(C4GRT_Vec4), "v1" }
		};
		C4GRT_Vec4 pout0[C4G_COUNTOF(in0)] = { 0.0f };
		C4GRT_Vec4 pout1[C4G_COUNTOF(in0)] = { 0.0f };
		const C4GRT_Data pout[] = {
			{ DT_VEC4, (unsigned char*)pout0, C4G_COUNTOF(pout0), sizeof(C4GRT_Vec4), "o0" },
			{ DT_VEC4, (unsigned char*)pout1, C4G_COUNTOF(pout1), sizeof(C4GRT_Vec4), "o1" }
		};
		C4GRT_Vec4 out0[C4G_COUNTOF(in0)] = { 0.0f };
		C4GRT_Vec4 out1[C4G_COUNTOF(in0)] = { 0.0f };
		const C4GRT_Data out[] = {
			{ DT_VEC4, (unsigned char*)out0, C4G_COUNTOF(out0), sizeof(C4GRT_Vec4), "o0" },
			{ DT_VEC4, (unsigned char*)out1, C4G_COUNTOF(out1), sizeof(C4GRT_Vec4), "o1" }
		};
		C4GRT_PassId p0 = r.addPass(rt, 0);
		C4GRT_PassId p1 = r.addPass(rt, p0);
		const char* const pipe[] = { "o0", "v0", "o1", "v1" };
		r.setPassPipe(rt, p0, true, pipe, C4G_COUNTOF(pipe));
		const char* const varyings[] = { "o0", "o1" };
		r.useGpuProgramFile(rt, p0, "../data/comp.vert", varyings, C4G_COUNTOF(varyings));
		r.useGpuProgramFile(rt, p1, "../data/comp.vert", varyings, C4G_COUNTOF(varyings));
		r.prepareBuffers(rt, p0, C4G_COUNTOF(ptx), C4G_COUNTOF(in), C4G_COUNTOF(pout));
		r.prepareBuffers(rt, p1, C4G_COUNTOF(tx), 0, C4G_COUNTOF(out));
		{
			r.prepareTex(rt, p0, ptx, C4G_COUNTOF(ptx));
			r.prepareTex(rt, p1, tx, C4G_COUNTOF(tx));
			r.prepareUniform(rt, p0, un, C4G_COUNTOF(un));
			r.prepareUniform(rt, p1, un, C4G_COUNTOF(un));
			r.prepareIn(rt, p0, in, C4G_COUNTOF(in));
			r.prepareOut(rt, p0, pout, C4G_COUNTOF(pout));
			r.prepareOut(rt, p1, out, C4G_COUNTOF(out));
			r.compute(rt, p0, false);
			r.mapOut(rt, p0);
			r.mapOut(rt, p1);
		}
		r.finish(rt, 0);
	}
	r.end(rt);

	r.close(rt);

	r.unload();

	return 0;
}
