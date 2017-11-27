local module = {
    dsc = 0,
    hp = 0,
    dp = 0,
    ap = 0,
    ar = 0,
    vr = 0,
    dpt = 0,
    is_name = "default_unit"
}
local module_mt = {__index = module}

function module:new()
    local inst = {}
    setmetatable(inst, module_mt)
    return inst
end

function module:get_hp() return self.hp end

function module:attack(enemy)
    adj = self.ap - lowest_sum(self.dsc, enemy.dsc, self, self.d_rel)
    d = enemy.dp - adj
    enemy.hp = enemy.hp + d
end

function module:attack_possible(tile)
    return (self.ar - lowest_sum(self.dsc, tile, self, self.a_rel)) > 0
end

function module:is_visible(tile)
    return (self.vr - lowest_sum(self.dsc, tile, self, self.v_rel)) > 0
end

function module:is_walking_distance(tile)
    return (self.dpt - lowest_sum(self.dsc, tile, self, self.t_rel)) > 0
end

return module
