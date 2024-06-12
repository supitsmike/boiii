if Engine.GetCurrentMap() ~= "core_frontend" then
  return
end

local utils = require("utils")

CoD.LobbyButtons.STATS = {
  stringRef = "STATS",
  action = function(self, element, controller, param, menu)
    SetPerControllerTableProperty(controller, "disableGameSettingsOptions", true)
    OpenPopup(menu, "GameSettings_Stats", controller)
  end,
  customId = "btnMPStats"
}

local addCustomButtons = function(controller, menuId, buttonTable, isLeader)
  if menuId == LobbyData.UITargets.UI_MPLOBBYONLINE.id or menuId == LobbyData.UITargets.UI_ZMLOBBYONLINE.id then
    utils.AddSmallButton(controller, buttonTable, CoD.LobbyButtons.STATS)
  end

  if menuId == LobbyData.UITargets.UI_ZMLOBBYONLINE.id then
    utils.AddSpacer(buttonTable, utils.GetButtonIndex(buttonTable, CoD.LobbyButtons.STATS))
  end
end

local oldAddButtonsForTarget = CoD.LobbyMenus.AddButtonsForTarget
CoD.LobbyMenus.AddButtonsForTarget = function(controller, id)
  local model = nil
  if Engine.IsLobbyActive(Enum.LobbyType.LOBBY_TYPE_GAME) then
    model = Engine.GetModel(DataSources.LobbyRoot.getModel(controller), "gameClient.isHost")
  else
    model = Engine.GetModel(DataSources.LobbyRoot.getModel(controller), "privateClient.isHost")
  end
  local isLeader = nil
  if model ~= nil then
    isLeader = Engine.GetModelValue(model)
  else
    isLeader = 1
  end
  local result = oldAddButtonsForTarget(controller, id)
  addCustomButtons(controller, id, result, isLeader)
  return result
end
