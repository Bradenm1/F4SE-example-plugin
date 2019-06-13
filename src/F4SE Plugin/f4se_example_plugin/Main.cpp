// F4SE
#include "f4se/PluginAPI.h"
#include "f4se/PapyrusNativeFunctions.h"

// Common
#include "f4se_common/f4se_version.h"

#include <shlobj.h>	// CSIDL_MYCODUMENTS

static F4SEPapyrusInterface* g_papyrus = NULL;

namespace Example_Plugin_Functions {
	void Test(StaticFunctionTag *base) {
		_MESSAGE("Hello World");
		Console_Print("Hello!");
	}
}

bool RegisterFuncs(VirtualMachine* vm) {
	vm->RegisterFunction(
		new NativeFunction0 <StaticFunctionTag, void>("Test", "TestClass", Example_Plugin_Functions::Test, vm));
	return true;
}

/* Plugin Query */

extern "C" {
	bool F4SEPlugin_Query(const F4SEInterface* f4se, PluginInfo* info) {
		gLog.OpenRelative(CSIDL_MYDOCUMENTS, "\\My Games\\Fallout4\\F4SE\\f4se_example_plugin.log");
		gLog.SetPrintLevel(IDebugLog::kLevel_Error);
		gLog.SetLogLevel(IDebugLog::kLevel_DebugMessage);

		// populate info structure
		info->infoVersion = PluginInfo::kInfoVersion;
		info->name = "f4se_example_plugin";
		info->version = 1;

		if (f4se->isEditor) {
			_MESSAGE("loaded in editor, marking as incompatible");

			return false;
		}
		else if (f4se->runtimeVersion != RUNTIME_VERSION_1_10_130) {
			_MESSAGE("unsupported runtime version %d", f4se->runtimeVersion);

			return false;
		}
		// ### do not do anything else in this callback
		// ### only fill out PluginInfo and return true/false

		_MESSAGE("F4SEPlugin_Query Loaded");

		// supported runtime version
		return true;
	}

	bool F4SEPlugin_Load(const F4SEInterface * f4se) {
		g_papyrus = (F4SEPapyrusInterface *)f4se->QueryInterface(kInterface_Papyrus);

		if (g_papyrus->Register(RegisterFuncs))
			_MESSAGE("F4SEPlugin_Load Funcs Registered");

		_MESSAGE("F4SEPlugin_Load Loaded");

		return true;
	}

};
