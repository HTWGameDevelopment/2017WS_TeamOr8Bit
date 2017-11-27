function load_script(name)
    tmp_module = require(name)
    return tmp_module.unitname, tmp_module.path
end
