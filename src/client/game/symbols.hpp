#pragma once

#include "structs.hpp"

#define WEAK __declspec(selectany)

namespace game
{
#define Com_Error(code, fmt, ...) Com_Error_(__FILE__, __LINE__, code, fmt, ##__VA_ARGS__)

	// CG
	WEAK symbol<void(int localClientNum, float* fov_x, float* dxDzAtDefaultAspectRatio, float* dxDz, float* dyDz)>
	CG_CalcFOVfromLens{0x1404D6230};

	// CL
	WEAK symbol<void(int controllerIndex, XSESSION_INFO* hostInfo, const netadr_t* addr, int numPublicSlots,
	                 int numPrivateSlots, const char* mapname, const char* gametype,
	                 const char* somethingWithUserMaps)> CL_ConnectFromLobby
		{0x14134C570};
	WEAK symbol<bool(int localClientNum, int index, char* buf, int size, bool addClanName)> CL_GetClientName{
		0x1413E3140
	};
	WEAK symbol<bool(int localClientNum)> CL_LocalClient_IsActive{0x14283AA50};
	WEAK symbol<float(void* key)> CL_KeyState{0x1412FF860};

	// Game
	WEAK symbol<void(gentity_s* ent, gentity_s* target, int mode, const char* chatText)> G_Say{ 0x1419765F0 };
	WEAK symbol<void(const char* fmt, ...)> G_LogPrintf{ 0x1419BFA50 };

	// Com
	WEAK symbol<void(int channel, unsigned int label, const char* fmt, ...)> Com_Printf{0x142148F60};
	WEAK symbol<void(const char* file, int line, int code, const char* fmt, ...)> Com_Error_{0x1420F8170};
	WEAK symbol<bool(eModes mode)> Com_SessionMode_IsMode{0x1420F7370};
	WEAK symbol<int()> Com_SessionMode_GetMode{0x1420F6D30};
	WEAK symbol<int()> Com_SessionMode_GetGameMode{0x1420F68B0};
	WEAK symbol<void(eNetworkModes networkMode)> Com_SessionMode_SetNetworkMode{0x1420F75B0};
	WEAK symbol<eGameModes(eGameModes gameMode)> Com_SessionMode_SetGameMode{0x1420F7570};
	WEAK symbol<eModes(eModes mode)> Com_SessionMode_SetMode{0x1420F7570};
	WEAK symbol<void(const char* gametype, bool loadDefaultSettings)> Com_GametypeSettings_SetGametype{
		0x1420F5980
	};
	WEAK symbol<unsigned int(const char* settingName, bool getDefault)> Com_GametypeSettings_GetUInt{
		0x1420F4E00
	};
	WEAK symbol<bool()> Com_IsRunningUILevel{0x142148350};
	WEAK symbol<bool()> Com_IsInGame{0x1421482C0};
	WEAK symbol<void(int localClientNum, eModes fromMode, eModes toMode, uint32_t flags)> Com_SwitchMode{
		0x14214A4D0
	};
	WEAK symbol<const char*(const char* fullpath)> Com_LoadRawTextFile{0x1420F61B0};

	WEAK symbol<void(int localClientNum, const char* text)> Cbuf_AddText{0x1420EC010};
	WEAK symbol<void(int localClientNum, ControllerIndex_t controllerIndex, const char* buffer)> Cbuf_ExecuteBuffer{
		0x14133BE10
	};
	WEAK symbol<void(const char* cmdName, xcommand_t function, cmd_function_s* allocedCmd)> Cmd_AddCommandInternal{
		0x1420ECC90
	};
	WEAK symbol<void()> Cbuf_AddServerText_f{0x1420EC000};
	WEAK symbol<void(const char* cmdName, xcommand_t function, cmd_function_s* allocedCmd)> Cmd_AddServerCommandInternal
	{
		0x1420ECD00
	};
	WEAK symbol<void(int localClientNum, ControllerIndex_t controllerIndex, const char* text,
	                 bool fromRemoteConsole)> Cmd_ExecuteSingleCommand{
		0x1420ED380
	};
	WEAK symbol<void(int localClientNum, ControllerIndex_t localControllerIndex, const char* text_in, int max_tokens,
	                 bool evalExpressions, CmdArgs* args)> Cmd_TokenizeStringKernel{0x1420EED60};
	WEAK symbol<void()> Cmd_EndTokenizedString{0x1420ECED0};
	WEAK symbol<void(char* text, int maxSize)> Con_GetTextCopy{0x14133A7D0};

	// DB
	WEAK symbol<void(XZoneInfo* zoneInfo, uint32_t zoneCount, bool sync, bool suppressSync)> DB_LoadXAssets{
		0x1414236A0
	};
	WEAK symbol<void(XAssetType type, XAssetEnum* func, void* inData, bool includeOverride)> DB_EnumXAssets{
		0x141420970
	};
	WEAK symbol<XAssetHeader(XAssetType type, const char* name, bool errorIfMissing, int waitTime)> DB_FindXAssetHeader{
		0x141420ED0
	};
	WEAK symbol<const char*(const XAsset* asset)> DB_GetXAssetName{0x1413E9DA0};
	WEAK symbol<bool(const char* zoneName, int source)> DB_FileExists{0x141420B40};
	WEAK symbol<void()> DB_ReleaseXAssets{0x1414247C0};

	// G
	WEAK symbol<void()> G_ClearVehicleInputs{0x1423812E0};

	WEAK symbol<qboolean(void* ent)> StuckInClient{0x1415A8360};

	WEAK symbol<qboolean(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)> InputLineWndProc{ 0x142332C60 };

	// Live
	WEAK symbol<bool(uint64_t, int*, bool)> Live_GetConnectivityInformation{0x141E0C380};

	// LiveStats
	WEAK symbol<const char*(const int controllerIndex)> LiveStats_GetClanTagText{0x141E9CE20};

	// Info
	WEAK symbol<const char*(const char*, const char* key)> Info_ValueForKey{0x1422E87B0};
	WEAK symbol<void(char* s, const char* key, const char* value)> Info_SetValueForKey{0x1422E8410};

	// MSG
	WEAK symbol<uint8_t(msg_t* msg)> MSG_ReadByte{0x142155450};

	// NET
	WEAK symbol<bool(netsrc_t sock, netadr_t* adr, const void* data, int len)> NET_OutOfBandData{0x142173600};
	WEAK symbol<bool(netsrc_t sock, int length, const void* data, const netadr_t* to)> NET_SendPacket{
		0x1423323B0
	};
	WEAK symbol<bool(char const*, netadr_t*)> NET_StringToAdr{0x142172780};

	// Sys
	WEAK symbol<int()> Sys_Milliseconds{0x142332870};
	WEAK symbol<void()> Sys_ShowConsole{0x1423333C0};
	WEAK symbol<TLSData*()> Sys_GetTLS{0x1421837B0};
	WEAK symbol<TLSData*()> Sys_IsDatabaseReady{0x142183A60};

	// Rope
	WEAK symbol<void()> Rope_InitRopes{0x1420D8D00};

	// Glass
	WEAK symbol<void()> GlassSv_Shutdown{0x1425AA7A0};

	// Mantle
	WEAK symbol<void()> Mantle_ShutdownAnims{0x142678C80};

	// Unnamed
	WEAK symbol<const char*(const char* name)> CopyString{0x1422AC220};

	WEAK symbol<bool()> isModLoaded{0x1420D5020};
	WEAK symbol<const char*()> getPublisherIdFromLoadedMod{0x1420D7680};
	WEAK symbol<void(int localClientNum, const char* mod, bool)> loadMod{0x1420D6930};
	WEAK symbol<void()> reloadUserContent{0x1420D66C0};

	// Dvar
	WEAK symbol<bool(const dvar_t* dvar)> Dvar_IsSessionModeBaseDvar{0x1422C23A0};
	WEAK symbol<dvar_t*(const char* dvarName)> Dvar_FindVar{0x1422BCCD0};
	WEAK symbol<unsigned int(const char* str)> Dvar_GenerateHash{0x14133DBF0};
	WEAK symbol<dvar_t*(unsigned int hash)> Dvar_FindMalleableVar{0x1422BCC40};
	WEAK symbol<const char*(const dvar_t* dvar)> Dvar_GetDebugName{0x1422BD250};
	WEAK symbol<const char*(const dvar_t* dvar)> Dvar_GetString{0x1422BF590};
	WEAK symbol<const char*(const dvar_t* dvar)> Dvar_DisplayableValue{0x1422BC080};
	WEAK symbol<bool(const dvar_t* dvar)> Dvar_GetBool{0x1422BCED0};
	WEAK symbol<int(const dvar_t* dvar)> Dvar_GetInt{0x1422BE860};
	WEAK symbol<float(const dvar_t* dvar)> Dvar_GetFloat{0x1422BD620};
	WEAK symbol<dvar_t*(dvarStrHash_t hash, const char* dvarName, bool value, int flags,
	                    const char* description)> Dvar_RegisterBool{
		0x1422D0900
	};
	WEAK symbol<dvar_t*(dvarStrHash_t hash, const char* dvarName, int value, int min, int max, unsigned int flags,
	                    const char* description)> Dvar_RegisterInt{
		0x1422D0AE0
	};
	WEAK symbol<dvar_t*(dvarStrHash_t hash, const char* dvarName, float value, float min, float max, unsigned int flags,
	                    const char* description)> Dvar_RegisterFloat{
		0x1422D0A60
	};
	WEAK symbol<dvar_t*(dvarStrHash_t hash, const char* dvarName, bool value, int flags,
	                    const char* description)> Dvar_SessionModeRegisterBool{
		0x1422D0D40
	};
	WEAK symbol<void(dvarStrHash_t hash, bool value, eModes mode)> Dvar_SessionModeSetDefaultBool{
		0x1422D0E90
	};
	WEAK symbol<dvar_t*(dvarStrHash_t hash, const char* dvarName, const char* value, int flags,
	                    const char* description)> Dvar_RegisterString{
		0x1422D0B70
	};
	WEAK symbol<void(void (*callback)(const dvar_t*, void*), void* userData)> Dvar_ForEach{0x1422BCD00};
	WEAK symbol<void(const char* dvarName, const char* string, bool createIfMissing)> Dvar_SetFromStringByName{
		0x1422C7500
	};
	WEAK symbol<dvar_t*(dvar_t* dvar, eModes mode)> Dvar_GetSessionModeSpecificDvar{
		0x1422BF500
	};

	// UI
	WEAK symbol<void(int localClientNumber, int errorcode, const char* errorMessage)> UI_OpenErrorPopupWithMessage{
		0x14228DEE0
	};
	WEAK symbol<void(bool frontend)> UI_CoD_Init{0x141F29010};
	WEAK symbol<void()> UI_CoD_LobbyUI_Init{0x141F2BD80};
	WEAK symbol<void()> UI_CoD_Shutdown{0x141F32E10};
	WEAK symbol<void(const char*, const char*, int, hks::lua_State*)> UI_AddMenu{0x1427018F0};
	WEAK symbol<const char*(int)> UI_CoD_GetRootNameForController{0x141F28940};
	WEAK symbol<void(hks::lua_State*, const char*)> Lua_CoD_LoadLuaFile{0x141F11A20};
	WEAK symbol<void(int localClientNum)> CG_LUIHUDRestart{0x140F7E970};
	WEAK symbol<void(int localClientNum)> CL_CheckKeepDrawingConnectScreen{0x1413CCAE0};
	WEAK symbol<void(const char* key, int value, hks::lua_State* luaVM)> Lua_SetTableInt{0x141F066E0};

	// Scr
	WEAK symbol<void(scriptInstance_t inst, int value)> Scr_AddInt{0x1412E9870};
	WEAK symbol<void(scriptInstance_t inst, const char* value)> Scr_AddString{0x1412E9A30};
	WEAK symbol<const char*(scriptInstance_t inst, unsigned int index)> Scr_GetString{0x1412EBAA0};
	WEAK symbol<void(gentity_s* ent, ScrVarCanonicalName_t stringValue, unsigned int paramcount)> Scr_Notify_Canon{
		0x141B46500
	};
	WEAK symbol<unsigned int(scriptInstance_t inst)> Scr_GetNumParam{0x1412EB930};
	WEAK symbol<unsigned int(scriptInstance_t inst, const char* filename)> Scr_LoadScript{0x1412C83F0};
	WEAK symbol<void(scriptInstance_t inst, int user)> Scr_BeginLoadScripts{0x1412C7DF0};

	WEAK symbol<void(const char* name, const char* key, unsigned int playbackFlags, float volume, void* callbackInfo,
	                 int id)> Cinematic_StartPlayback{0x1412BE3A0};
	WEAK symbol<void(uint64_t id, bool cancelAll)> Cinematic_StopPlayback{0x1412BEA70};

	// Rendering
	WEAK symbol<void(const char*, int, const void*, float, float, float, float, float, const float*, int)>
	R_AddCmdDrawText{
		0x141CD98D0
	};

	// PCache
	WEAK symbol<void(ControllerIndex_t controllerIndex)> PCache_DeleteEntries{0x141E8D710};

	// SV
	WEAK symbol<bool()> SV_Loaded{0x142252250};
	WEAK symbol<void*()> SV_AddTestClient{0x142248F40};
	WEAK symbol<void (netadr_t from)> SV_DirectConnect{0x142249880};
	WEAK symbol<void(int clientNum, svscmd_type type, const char* text)> SV_GameSendServerCommand{
		0x14224F580
	};
	WEAK symbol<void(client_s* cl_0, svscmd_type type, const char* fmt, ...)> SV_SendServerCommand{0x14224F580};
	WEAK symbol<bool(int clientNum)> SV_IsTestClient{0x14224AB60};
	WEAK symbol<void(int controllerIndex, const char* server, MapPreload preload, bool savegame)> SV_SpawnServer{
		0x1422528C0
	};
	WEAK symbol<void(const char* text_in)> SV_Cmd_TokenizeString{0x1420EF130};
	WEAK symbol<void()> SV_Cmd_EndTokenizedString{0x1420EF0E0};
	WEAK symbol<void(void* client, msg_t* msg)> SV_ExecuteClientMessage{0x14224A460};

	WEAK symbol<void(void* drop, const char* reason, bool tellThem, bool removeFromLobby)> SV_DropClient{
		0x14224A050
	};

	// FS
	WEAK symbol<char*(int bytes)> FS_AllocMem{0x1422AC9F0};
	WEAK symbol<fileHandle_t(const char* filename, const char* dir, const char* osbasepath)> FS_FOpenFileWriteToDir{
		0x1422A35D0
	};
	WEAK symbol<fileHandle_t(const char* filename, const char* dir, const char* osbasepath)> FS_FOpenFileReadFromDir{
		0x1422A3510
	};

	// Lobby
	WEAK symbol<int(LobbyType lobbyType, LobbyClientType clientType)> LobbyHost_GetClientCount{
		0x141ED8AC0
	};

	// Utils
	WEAK symbol<const char*(char* str)> I_CleanStr{0x1422E9050};
	WEAK symbol<int(const char* s0, const char* s1)> I_stricmp{0x1422E9530};
	WEAK symbol<void(char* dest, size_t destsize, const char* src)> I_strcpy{
		0x1422E9410
	};

	// Variables
	WEAK symbol<cmd_function_s> cmd_functions{0x15689DF58};
	WEAK symbol<CmdArgs> sv_cmd_args{0x15689AE30};

	WEAK symbol<gentity_s> g_entities{0x14A5F25F8};

	WEAK symbol<int> level_time{0x14A5502C4};
	WEAK symbol<int> level_rounds_played{0x14A55BDEC};

	WEAK symbol<SOCKET> ip_socket{0x157E75818};

	WEAK symbol<Join> s_join{0x15574A640};

	WEAK symbol<char> s_dvarPool{0x157AC6220};
	WEAK symbol<int> g_dvarCount{0x157AC61CC};

	WEAK symbol<unsigned int> modsCount{0x15678D170};
	WEAK symbol<workshop_data> modsPool{0x15678D178};

	WEAK symbol<unsigned int> usermapsCount{0x1567B3580};
	WEAK symbol<workshop_data> usermapsPool{0x1567B3588};

	WEAK symbol<int> fs_loadStack{0x157A65310};

	WEAK symbol<client_s*> svs_clients{0x1576F9318};

	// Dvar variables
	WEAK symbol<dvar_t*> com_maxclients{0x1568ED850};

	WEAK symbol<clientUIActive_t> clientUIActives{0x1453D8BC0};

	namespace s_wcd
	{
		WEAK symbol<HWND> codLogo{0x157E75A50};
		WEAK symbol<HFONT> hfBufferFont{0x157E75A58};
		WEAK symbol<HWND> hWnd{0x157E75A40};
		WEAK symbol<HWND> hwndBuffer{0x157E75A48};
		WEAK symbol<HWND> hwndInputLine{0x157E75A60};
		WEAK symbol<int> windowHeight{0x157E7606C};
		WEAK symbol<int> windowWidth{0x157E76068};
		WEAK symbol<WNDPROC> SysInputLineWndProc{0x157E76070};
	}

	// Global game definitions
	constexpr auto CMD_MAX_NESTING = 8;

	// Re-implementations
	bool I_islower(int c);
	bool I_isupper(int c);

	unsigned int Scr_CanonHash(const char* str);

	namespace hks
	{
		WEAK symbol<lua_State*> lua_state{0x159C76D88};
		WEAK symbol<void(lua_State* s, const char* str, unsigned int l)> hksi_lua_pushlstring{0x140A18430};

		WEAK symbol<void(lua_State* s, const HksObject* tbl, const HksObject* key, const HksObject* val)>
		hks_obj_settable{0x141D4B660};
		WEAK symbol<HksObject*(HksObject* result, lua_State* s, const HksObject* table, const HksObject* key)>
		hks_obj_gettable{0x141D4ABF0};
		WEAK symbol<void(lua_State* s, int nargs, int nresults, const unsigned int* pc)> vm_call_internal{
			0x141D70FE0
		};
		WEAK symbol<HashTable*(lua_State* s, unsigned int arraySize, unsigned int hashSize)> Hashtable_Create{
			0x141D3B5F0
		};
		WEAK symbol<cclosure*(lua_State* s, lua_function function, int num_upvalues, int internal_,
		                      int profilerTreatClosureAsFunc)> cclosure_Create{0x141D3B7E0};
		WEAK symbol<int(lua_State* s, int t)> hksi_luaL_ref{0x141D4D1A0};
		WEAK symbol<void(lua_State* s, int t, int ref)> hksi_luaL_unref{0x141D4D320};

		WEAK symbol<int(lua_State* s, const HksCompilerSettings* options, const char* buff, unsigned __int64 sz,
		                const char* name)> hksi_hksL_loadbuffer{0x141D4BD80};
		WEAK symbol<int(lua_State* s, const char* what, lua_Debug* ar)> hksi_lua_getinfo{0x141D4D8D0};
		WEAK symbol<int(lua_State* s, int level, lua_Debug* ar)> hksi_lua_getstack{0x141D4DB90};
		WEAK symbol<void(lua_State* s, const char* fmt, ...)> hksi_luaL_error{0x141D4D050};
		WEAK symbol<const char*> s_compilerTypeName{0x140A18430};
	}
}
