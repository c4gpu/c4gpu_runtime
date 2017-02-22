#include "c4g_context_win.hpp"
#include "c4g_gl.hpp"

namespace c4g {

namespace gl {

OpenGL::OpenGL() {
}

OpenGL::~OpenGL() {
}

bool OpenGL::open(void) {
	_context = createContext();

	glewInit();

	int major, minor;
	c4g::gl::OpenGL::getVersion(&major, &minor);
	if (major < 2) {
		printf("Support for OpenGL 2.0 is required.\n");

		finishCreatingContext(_context);

		return false;
	}

	setupGLParameters();

	finishCreatingContext(_context);

	return true;
}

bool OpenGL::close(void) {
	destroyContext(_context);

	return true;
}

bool OpenGL::begin(void) {
	pushContext(_context);

	return true;
}

bool OpenGL::end(void) {
	popContext(_context);

	return true;
}

void OpenGL::showDriverInfo(void) const {
	char device[256];
	c4g::gl::OpenGL::getDevice(device, sizeof(device));
	printf(device);
	int major, minor;
	c4g::gl::OpenGL::getVersion(&major, &minor);
	printf("OpenGL Version: %d.%d.\n", major, minor);
}

C4GRT_PassId OpenGL::addPass(C4GRT_PassId prev) {
	C4GRT_PassId result = _passIdSeed++;
	if (!result)
		result = _passIdSeed++;

	_passes[result] = new Pass(this, result);

	Pass* ppass = getPass(prev);
	if (ppass)
		ppass->next(result);

	if (!_headPass)
		_headPass = result;

	return result;
}

Pass* OpenGL::getPass(C4GRT_PassId id) {
	auto it = _passes.find(id);
	if (it == _passes.end())
		return nullptr;

	return it->second;
}

bool OpenGL::compute(C4GRT_PassId head, bool mapImm) {
	if (_passes.empty()) return false;

	C4GRT_PassId id = head ? head : _headPass;
	while (id) {
		Pass* p = getPass(id);
		if (!p) break;
		if (!p->compute(mapImm)) return false;
		id = p->next();
	}

	return true;
}

size_t OpenGL::finishAll(void) {
	size_t result = 0;
	for (auto it : _passes) {
		if (it.second->finish())
			++result;
	}

	return result;
}

void OpenGL::getDevice(char* device, size_t ds) {
	const char* vendor = (const char*)glGetString(GL_VENDOR);
	const char* renderer = (const char*)glGetString(GL_RENDERER);
	if (vendor && renderer) {
		snprintf(device, ds, "Device: %s by %s.\n", renderer, vendor);
	}
}

void OpenGL::getVersion(int* major, int* minor) {
	const char* verstr = (const char*)glGetString(GL_VERSION);
	if (!verstr || sscanf(verstr, "%d.%d", major, minor) != 2) {
		*major = *minor = 0;
		fprintf(stderr, "Invalid GL_VERSION format.\n");
	}
}

void OpenGL::setupGLParameters(void) {
	glEnable(GL_TEXTURE_1D);
	glEnable(GL_TEXTURE_2D);
	glEnable(GL_TEXTURE_3D);
}

}

}
