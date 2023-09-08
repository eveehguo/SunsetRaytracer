defaultMat = gr.material({0.5, 0.5, 0.5}, {0, 0, 0}, 0)

-- for convenience when defining rotations, easings
x = "x"
y = "y"
z = "z"

linear = 0
easeInQuad = 1
easeOutQuad = 2
easeInOutQuad = 3


-- helper function to parse scene graph from a lua table data structure
-- 
-- node type is inferred from name prefix:
--  n_ means a basic scene node
--  k_ means a keyframe node
--  m_ means a mesh (geometry) node
--  c_ means a cube node
--  s_ means a sphere node
-- 
function parseNode(nodeTable)
    local nameError = " : First indexed field must be the node name"
    local namePrefixError = " : Invalid node name prefix: must be one of 'n_', 'g_', 'j_'"

    -- validate node name (first indexed field)
    local name = nodeTable[1]
    assert(type(name) == "string", tostring(name) .. nameError)
    assert(name:sub(2,2) == "_", name .. namePrefixError) -- enforce name prefix with 2nd character being "_"
    
    -- infer node type from node name and create node
    local nodeType = name:sub(1,1)
    local node

    if nodeType == "n" then
        node = gr.node(name)
    elseif nodeType == "k" then
        node = gr.keyframe(name,
                nodeTable.t0 or 0,
                nodeTable.t1 or 1,
                nodeTable.ease or linear)
    elseif nodeType == "m" then
        node = gr.mesh(name, nodeTable.mesh or "cube.obj") -- mesh is optional; defaults to a cube
        node:set_material(nodeTable.mat or defaultMat) -- ensure a material is set
    elseif nodeType == "c" then
        node = gr.cube(name)
        node:set_material(nodeTable.mat or defaultMat) -- ensure a material is set
    else
        assert(nodeType == "s", name .. namePrefixError)
        node = gr.sphere(name)
        node:set_material(nodeTable.mat or defaultMat) -- ensure a material is set
    end

    -- apply transformations, in the following order: scale, rotate, translate
    if nodeTable.sca then
        if type(nodeTable.sca) == "number" then -- allow sca=N as a shorthand for sca={N, N, N}
            node:scale(nodeTable.sca, nodeTable.sca, nodeTable.sca)
        elseif #nodeTable.sca > 0 then -- ignore empty {} transforms
            node:scale(table.unpack(nodeTable.sca))
        end
    end

    if nodeTable.rot then -- allow multiple rotations, defined in sequence (also ignores empty {} rotation)
        local i = 1
        while i < #nodeTable.rot do
            node:rotate(nodeTable.rot[i], nodeTable.rot[i + 1])
            i = i + 2
        end
    end

    if nodeTable.tra and #nodeTable.tra > 0 then
        node:translate(table.unpack(nodeTable.tra))
    end

    -- parse child nodes (any additional indexed fields)
    local i = 2;
    while nodeTable[i] do
        local child = parseNode(nodeTable[i])
        node:add_child(child)
        i = i + 1
    end

    return node
end
