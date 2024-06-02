#include <std_include.hpp>

#include "proxy.hpp"

#include <utils/nt.hpp>

namespace proxy
{
	struct d3d11_dll {
		utils::nt::library dll{};
		FARPROC OriginalD3D11CreateDeviceForD3D12{};
		FARPROC OriginalD3DKMTCloseAdapter{};
		FARPROC OriginalD3DKMTDestroyAllocation{};
		FARPROC OriginalD3DKMTDestroyContext{};
		FARPROC OriginalD3DKMTDestroyDevice{};
		FARPROC OriginalD3DKMTDestroySynchronizationObject{};
		FARPROC OriginalD3DKMTPresent{};
		FARPROC OriginalD3DKMTQueryAdapterInfo{};
		FARPROC OriginalD3DKMTSetDisplayPrivateDriverFormat{};
		FARPROC OriginalD3DKMTSignalSynchronizationObject{};
		FARPROC OriginalD3DKMTUnlock{};
		FARPROC OriginalD3DKMTWaitForSynchronizationObject{};
		FARPROC OriginalEnableFeatureLevelUpgrade{};
		FARPROC OriginalOpenAdapter10{};
		FARPROC OriginalOpenAdapter10_2{};
		FARPROC OriginalCreateDirect3D11DeviceFromDXGIDevice{};
		FARPROC OriginalCreateDirect3D11SurfaceFromDXGISurface{};
		FARPROC OriginalD3D11CoreCreateDevice{};
		FARPROC OriginalD3D11CoreCreateLayeredDevice{};
		FARPROC OriginalD3D11CoreGetLayeredDeviceSize{};
		FARPROC OriginalD3D11CoreRegisterLayers{};
		FARPROC OriginalD3D11CreateDevice{};
		FARPROC OriginalD3D11CreateDeviceAndSwapChain{};
		FARPROC OriginalD3D11On12CreateDevice{};
		FARPROC OriginalD3DKMTCreateAllocation{};
		FARPROC OriginalD3DKMTCreateContext{};
		FARPROC OriginalD3DKMTCreateDevice{};
		FARPROC OriginalD3DKMTCreateSynchronizationObject{};
		FARPROC OriginalD3DKMTEscape{};
		FARPROC OriginalD3DKMTGetContextSchedulingPriority{};
		FARPROC OriginalD3DKMTGetDeviceState{};
		FARPROC OriginalD3DKMTGetDisplayModeList{};
		FARPROC OriginalD3DKMTGetMultisampleMethodList{};
		FARPROC OriginalD3DKMTGetRuntimeData{};
		FARPROC OriginalD3DKMTGetSharedPrimaryHandle{};
		FARPROC OriginalD3DKMTLock{};
		FARPROC OriginalD3DKMTOpenAdapterFromHdc{};
		FARPROC OriginalD3DKMTOpenResource{};
		FARPROC OriginalD3DKMTQueryAllocationResidency{};
		FARPROC OriginalD3DKMTQueryResourceInfo{};
		FARPROC OriginalD3DKMTRender{};
		FARPROC OriginalD3DKMTSetAllocationPriority{};
		FARPROC OriginalD3DKMTSetContextSchedulingPriority{};
		FARPROC OriginalD3DKMTSetDisplayMode{};
		FARPROC OriginalD3DKMTSetGammaRamp{};
		FARPROC OriginalD3DKMTSetVidPnSourceOwner{};
		FARPROC OriginalD3DKMTWaitForVerticalBlankEvent{};
		FARPROC OriginalD3DPerformance_BeginEvent{};
		FARPROC OriginalD3DPerformance_EndEvent{};
		FARPROC OriginalD3DPerformance_GetStatus{};
		FARPROC OriginalD3DPerformance_SetMarker{};
	} d3d11;

	extern "C"
	{
		FARPROC ProcAddress = nullptr;
		int run_proxy();

		void FakeD3D11CreateDeviceForD3D12() { ProcAddress = d3d11.OriginalD3D11CreateDeviceForD3D12; run_proxy(); }
		void FakeD3DKMTCloseAdapter() { ProcAddress = d3d11.OriginalD3DKMTCloseAdapter; run_proxy(); }
		void FakeD3DKMTDestroyAllocation() { ProcAddress = d3d11.OriginalD3DKMTDestroyAllocation; run_proxy(); }
		void FakeD3DKMTDestroyContext() { ProcAddress = d3d11.OriginalD3DKMTDestroyContext; run_proxy(); }
		void FakeD3DKMTDestroyDevice() { ProcAddress = d3d11.OriginalD3DKMTDestroyDevice; run_proxy(); }
		void FakeD3DKMTDestroySynchronizationObject() { ProcAddress = d3d11.OriginalD3DKMTDestroySynchronizationObject; run_proxy(); }
		void FakeD3DKMTPresent() { ProcAddress = d3d11.OriginalD3DKMTPresent; run_proxy(); }
		void FakeD3DKMTQueryAdapterInfo() { ProcAddress = d3d11.OriginalD3DKMTQueryAdapterInfo; run_proxy(); }
		void FakeD3DKMTSetDisplayPrivateDriverFormat() { ProcAddress = d3d11.OriginalD3DKMTSetDisplayPrivateDriverFormat; run_proxy(); }
		void FakeD3DKMTSignalSynchronizationObject() { ProcAddress = d3d11.OriginalD3DKMTSignalSynchronizationObject; run_proxy(); }
		void FakeD3DKMTUnlock() { ProcAddress = d3d11.OriginalD3DKMTUnlock; run_proxy(); }
		void FakeD3DKMTWaitForSynchronizationObject() { ProcAddress = d3d11.OriginalD3DKMTWaitForSynchronizationObject; run_proxy(); }
		void FakeEnableFeatureLevelUpgrade() { ProcAddress = d3d11.OriginalEnableFeatureLevelUpgrade; run_proxy(); }
		void FakeOpenAdapter10() { ProcAddress = d3d11.OriginalOpenAdapter10; run_proxy(); }
		void FakeOpenAdapter10_2() { ProcAddress = d3d11.OriginalOpenAdapter10_2; run_proxy(); }
		void FakeCreateDirect3D11DeviceFromDXGIDevice() { ProcAddress = d3d11.OriginalCreateDirect3D11DeviceFromDXGIDevice; run_proxy(); }
		void FakeCreateDirect3D11SurfaceFromDXGISurface() { ProcAddress = d3d11.OriginalCreateDirect3D11SurfaceFromDXGISurface; run_proxy(); }
		void FakeD3D11CoreCreateDevice() { ProcAddress = d3d11.OriginalD3D11CoreCreateDevice; run_proxy(); }
		void FakeD3D11CoreCreateLayeredDevice() { ProcAddress = d3d11.OriginalD3D11CoreCreateLayeredDevice; run_proxy(); }
		void FakeD3D11CoreGetLayeredDeviceSize() { ProcAddress = d3d11.OriginalD3D11CoreGetLayeredDeviceSize; run_proxy(); }
		void FakeD3D11CoreRegisterLayers() { ProcAddress = d3d11.OriginalD3D11CoreRegisterLayers; run_proxy(); }
		void FakeD3D11CreateDevice() { ProcAddress = d3d11.OriginalD3D11CreateDevice; run_proxy(); }
		void FakeD3D11CreateDeviceAndSwapChain() { ProcAddress = d3d11.OriginalD3D11CreateDeviceAndSwapChain; run_proxy(); }
		void FakeD3D11On12CreateDevice() { ProcAddress = d3d11.OriginalD3D11On12CreateDevice; run_proxy(); }
		void FakeD3DKMTCreateAllocation() { ProcAddress = d3d11.OriginalD3DKMTCreateAllocation; run_proxy(); }
		void FakeD3DKMTCreateContext() { ProcAddress = d3d11.OriginalD3DKMTCreateContext; run_proxy(); }
		void FakeD3DKMTCreateDevice() { ProcAddress = d3d11.OriginalD3DKMTCreateDevice; run_proxy(); }
		void FakeD3DKMTCreateSynchronizationObject() { ProcAddress = d3d11.OriginalD3DKMTCreateSynchronizationObject; run_proxy(); }
		void FakeD3DKMTEscape() { ProcAddress = d3d11.OriginalD3DKMTEscape; run_proxy(); }
		void FakeD3DKMTGetContextSchedulingPriority() { ProcAddress = d3d11.OriginalD3DKMTGetContextSchedulingPriority; run_proxy(); }
		void FakeD3DKMTGetDeviceState() { ProcAddress = d3d11.OriginalD3DKMTGetDeviceState; run_proxy(); }
		void FakeD3DKMTGetDisplayModeList() { ProcAddress = d3d11.OriginalD3DKMTGetDisplayModeList; run_proxy(); }
		void FakeD3DKMTGetMultisampleMethodList() { ProcAddress = d3d11.OriginalD3DKMTGetMultisampleMethodList; run_proxy(); }
		void FakeD3DKMTGetRuntimeData() { ProcAddress = d3d11.OriginalD3DKMTGetRuntimeData; run_proxy(); }
		void FakeD3DKMTGetSharedPrimaryHandle() { ProcAddress = d3d11.OriginalD3DKMTGetSharedPrimaryHandle; run_proxy(); }
		void FakeD3DKMTLock() { ProcAddress = d3d11.OriginalD3DKMTLock; run_proxy(); }
		void FakeD3DKMTOpenAdapterFromHdc() { ProcAddress = d3d11.OriginalD3DKMTOpenAdapterFromHdc; run_proxy(); }
		void FakeD3DKMTOpenResource() { ProcAddress = d3d11.OriginalD3DKMTOpenResource; run_proxy(); }
		void FakeD3DKMTQueryAllocationResidency() { ProcAddress = d3d11.OriginalD3DKMTQueryAllocationResidency; run_proxy(); }
		void FakeD3DKMTQueryResourceInfo() { ProcAddress = d3d11.OriginalD3DKMTQueryResourceInfo; run_proxy(); }
		void FakeD3DKMTRender() { ProcAddress = d3d11.OriginalD3DKMTRender; run_proxy(); }
		void FakeD3DKMTSetAllocationPriority() { ProcAddress = d3d11.OriginalD3DKMTSetAllocationPriority; run_proxy(); }
		void FakeD3DKMTSetContextSchedulingPriority() { ProcAddress = d3d11.OriginalD3DKMTSetContextSchedulingPriority; run_proxy(); }
		void FakeD3DKMTSetDisplayMode() { ProcAddress = d3d11.OriginalD3DKMTSetDisplayMode; run_proxy(); }
		void FakeD3DKMTSetGammaRamp() { ProcAddress = d3d11.OriginalD3DKMTSetGammaRamp; run_proxy(); }
		void FakeD3DKMTSetVidPnSourceOwner() { ProcAddress = d3d11.OriginalD3DKMTSetVidPnSourceOwner; run_proxy(); }
		void FakeD3DKMTWaitForVerticalBlankEvent() { ProcAddress = d3d11.OriginalD3DKMTWaitForVerticalBlankEvent; run_proxy(); }
		void FakeD3DPerformance_BeginEvent() { ProcAddress = d3d11.OriginalD3DPerformance_BeginEvent; run_proxy(); }
		void FakeD3DPerformance_EndEvent() { ProcAddress = d3d11.OriginalD3DPerformance_EndEvent; run_proxy(); }
		void FakeD3DPerformance_GetStatus() { ProcAddress = d3d11.OriginalD3DPerformance_GetStatus; run_proxy(); }
		void FakeD3DPerformance_SetMarker() { ProcAddress = d3d11.OriginalD3DPerformance_SetMarker; run_proxy(); }
	}

	void setup_proxy()
	{
		char dir[MAX_PATH];
		GetSystemDirectory(dir, sizeof(dir));

		ZeroMemory(&d3d11, sizeof(d3d11_dll));
		d3d11.dll = utils::nt::library::load(dir + "/d3d11.dll"s);
		if (d3d11.dll == nullptr) throw std::runtime_error("Failed to load original d3d11.dll library.");

		d3d11.OriginalD3D11CreateDeviceForD3D12 = d3d11.dll.get_proc<FARPROC>("D3D11CreateDeviceForD3D12");
		d3d11.OriginalD3DKMTCloseAdapter = d3d11.dll.get_proc<FARPROC>("D3DKMTCloseAdapter");
		d3d11.OriginalD3DKMTDestroyAllocation = d3d11.dll.get_proc<FARPROC>("D3DKMTDestroyAllocation");
		d3d11.OriginalD3DKMTDestroyContext = d3d11.dll.get_proc<FARPROC>("D3DKMTDestroyContext");
		d3d11.OriginalD3DKMTDestroyDevice = d3d11.dll.get_proc<FARPROC>("D3DKMTDestroyDevice");
		d3d11.OriginalD3DKMTDestroySynchronizationObject = d3d11.dll.get_proc<FARPROC>("D3DKMTDestroySynchronizationObject");
		d3d11.OriginalD3DKMTPresent = d3d11.dll.get_proc<FARPROC>("D3DKMTPresent");
		d3d11.OriginalD3DKMTQueryAdapterInfo = d3d11.dll.get_proc<FARPROC>("D3DKMTQueryAdapterInfo");
		d3d11.OriginalD3DKMTSetDisplayPrivateDriverFormat = d3d11.dll.get_proc<FARPROC>("D3DKMTSetDisplayPrivateDriverFormat");
		d3d11.OriginalD3DKMTSignalSynchronizationObject = d3d11.dll.get_proc<FARPROC>("D3DKMTSignalSynchronizationObject");
		d3d11.OriginalD3DKMTUnlock = d3d11.dll.get_proc<FARPROC>("D3DKMTUnlock");
		d3d11.OriginalD3DKMTWaitForSynchronizationObject = d3d11.dll.get_proc<FARPROC>("D3DKMTWaitForSynchronizationObject");
		d3d11.OriginalEnableFeatureLevelUpgrade = d3d11.dll.get_proc<FARPROC>("EnableFeatureLevelUpgrade");
		d3d11.OriginalOpenAdapter10 = d3d11.dll.get_proc<FARPROC>("OpenAdapter10");
		d3d11.OriginalOpenAdapter10_2 = d3d11.dll.get_proc<FARPROC>("OpenAdapter10_2");
		d3d11.OriginalCreateDirect3D11DeviceFromDXGIDevice = d3d11.dll.get_proc<FARPROC>("CreateDirect3D11DeviceFromDXGIDevice");
		d3d11.OriginalCreateDirect3D11SurfaceFromDXGISurface = d3d11.dll.get_proc<FARPROC>("CreateDirect3D11SurfaceFromDXGISurface");
		d3d11.OriginalD3D11CoreCreateDevice = d3d11.dll.get_proc<FARPROC>("D3D11CoreCreateDevice");
		d3d11.OriginalD3D11CoreCreateLayeredDevice = d3d11.dll.get_proc<FARPROC>("D3D11CoreCreateLayeredDevice");
		d3d11.OriginalD3D11CoreGetLayeredDeviceSize = d3d11.dll.get_proc<FARPROC>("D3D11CoreGetLayeredDeviceSize");
		d3d11.OriginalD3D11CoreRegisterLayers = d3d11.dll.get_proc<FARPROC>("D3D11CoreRegisterLayers");
		d3d11.OriginalD3D11CreateDevice = d3d11.dll.get_proc<FARPROC>("D3D11CreateDevice");
		d3d11.OriginalD3D11CreateDeviceAndSwapChain = d3d11.dll.get_proc<FARPROC>("D3D11CreateDeviceAndSwapChain");
		d3d11.OriginalD3D11On12CreateDevice = d3d11.dll.get_proc<FARPROC>("D3D11On12CreateDevice");
		d3d11.OriginalD3DKMTCreateAllocation = d3d11.dll.get_proc<FARPROC>("D3DKMTCreateAllocation");
		d3d11.OriginalD3DKMTCreateContext = d3d11.dll.get_proc<FARPROC>("D3DKMTCreateContext");
		d3d11.OriginalD3DKMTCreateDevice = d3d11.dll.get_proc<FARPROC>("D3DKMTCreateDevice");
		d3d11.OriginalD3DKMTCreateSynchronizationObject = d3d11.dll.get_proc<FARPROC>("D3DKMTCreateSynchronizationObject");
		d3d11.OriginalD3DKMTEscape = d3d11.dll.get_proc<FARPROC>("D3DKMTEscape");
		d3d11.OriginalD3DKMTGetContextSchedulingPriority = d3d11.dll.get_proc<FARPROC>("D3DKMTGetContextSchedulingPriority");
		d3d11.OriginalD3DKMTGetDeviceState = d3d11.dll.get_proc<FARPROC>("D3DKMTGetDeviceState");
		d3d11.OriginalD3DKMTGetDisplayModeList = d3d11.dll.get_proc<FARPROC>("D3DKMTGetDisplayModeList");
		d3d11.OriginalD3DKMTGetMultisampleMethodList = d3d11.dll.get_proc<FARPROC>("D3DKMTGetMultisampleMethodList");
		d3d11.OriginalD3DKMTGetRuntimeData = d3d11.dll.get_proc<FARPROC>("D3DKMTGetRuntimeData");
		d3d11.OriginalD3DKMTGetSharedPrimaryHandle = d3d11.dll.get_proc<FARPROC>("D3DKMTGetSharedPrimaryHandle");
		d3d11.OriginalD3DKMTLock = d3d11.dll.get_proc<FARPROC>("D3DKMTLock");
		d3d11.OriginalD3DKMTOpenAdapterFromHdc = d3d11.dll.get_proc<FARPROC>("D3DKMTOpenAdapterFromHdc");
		d3d11.OriginalD3DKMTOpenResource = d3d11.dll.get_proc<FARPROC>("D3DKMTOpenResource");
		d3d11.OriginalD3DKMTQueryAllocationResidency = d3d11.dll.get_proc<FARPROC>("D3DKMTQueryAllocationResidency");
		d3d11.OriginalD3DKMTQueryResourceInfo = d3d11.dll.get_proc<FARPROC>("D3DKMTQueryResourceInfo");
		d3d11.OriginalD3DKMTRender = d3d11.dll.get_proc<FARPROC>("D3DKMTRender");
		d3d11.OriginalD3DKMTSetAllocationPriority = d3d11.dll.get_proc<FARPROC>("D3DKMTSetAllocationPriority");
		d3d11.OriginalD3DKMTSetContextSchedulingPriority = d3d11.dll.get_proc<FARPROC>("D3DKMTSetContextSchedulingPriority");
		d3d11.OriginalD3DKMTSetDisplayMode = d3d11.dll.get_proc<FARPROC>("D3DKMTSetDisplayMode");
		d3d11.OriginalD3DKMTSetGammaRamp = d3d11.dll.get_proc<FARPROC>("D3DKMTSetGammaRamp");
		d3d11.OriginalD3DKMTSetVidPnSourceOwner = d3d11.dll.get_proc<FARPROC>("D3DKMTSetVidPnSourceOwner");
		d3d11.OriginalD3DKMTWaitForVerticalBlankEvent = d3d11.dll.get_proc<FARPROC>("D3DKMTWaitForVerticalBlankEvent");
		d3d11.OriginalD3DPerformance_BeginEvent = d3d11.dll.get_proc<FARPROC>("D3DPerformance_BeginEvent");
		d3d11.OriginalD3DPerformance_EndEvent = d3d11.dll.get_proc<FARPROC>("D3DPerformance_EndEvent");
		d3d11.OriginalD3DPerformance_GetStatus = d3d11.dll.get_proc<FARPROC>("D3DPerformance_GetStatus");
		d3d11.OriginalD3DPerformance_SetMarker = d3d11.dll.get_proc<FARPROC>("D3DPerformance_SetMarker");
	}
}
