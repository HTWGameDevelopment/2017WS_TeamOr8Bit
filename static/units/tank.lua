local default_unit = require "static/units/default_unit"

local module = {
    unitname = "tank",
    path = "./assets/models/tank.objv1",
    is = default_unit
}
local module_mt = {__index = module}

function module:new()
    local inst = {
        hp = 50,
        dp = 20,
        ap = 25,
        ar = 3,
        vr = 5,
        dpt = 3
    }

    setmetatable(inst, {__index = module.is})
    return inst
end

return module
