require "common"

-- gr.material({kd}, {ks}, shininess, reflectivity, transparency, refractiveIndex, glossiness)

grass = gr.material({0.28, 0.56, 0.24}, {0, 0, 0}, 0)
dirt = gr.material({0.25, 0.12, 0.04}, {0, 0, 0}, 0)

road = gr.material({0.2, 0.2, 0.2}, {0.2, 0.2, 0.2}, 200)
median = gr.material({0.5, 0.5, 0.2}, {0.4, 0.4, 0.4}, 100)
concrete = gr.material({0.6, 0.59, 0.6}, {0.1, 0.1, 0.1}, 5)

buildingFloor = gr.material({0.85, 0.8, 0.75}, {0.5, 0.5, 0.5}, 25)
buildingTrim = gr.material({0.5, 0.6, 0.7}, {0.5, 0.5, 0.5}, 25)
buildingGlass = gr.material({0.6, 0.9, 0.9}, {0.5, 0.5, 0.5}, 100, 0.9, 0.9, 1.5)

train = gr.material({0.9, 0.9, 0.9}, {0.7, 0.7, 0.7}, 50)
trainGlass = gr.material({0.4, 0.5, 0.5}, {0.4, 0.6, 0.4}, 100, 0.8, 0.7, 1.5)

water = gr.material({0.2, 0.2, 0.8}, {0.8, 0.8, 0.8}, 200, 0.8, 0.8, 1.33)

statue = gr.material({0.4, 0.8, 0.6}, {0.8, 0.8, 0.8}, 100, 0.9, 0.9, 1.4)

defaultMat = buildingFloor


windowThickness=0.25
trimThickness=1
function BuildingFacade(levels, width, columns, tra_, rot_)
    local t = {
        "n_buildingFacade",
        tra=tra_,
        rot=rot_,
    }
    for i = 1, #columns do
        table.insert(t, {
            "c_buildingColumn",
            mat=buildingTrim,
            tra={columns[i]*width - trimThickness/2, 0, -trimThickness},
            sca={trimThickness, 2+levels*10, width+trimThickness*2},
        })
    end
    return t
end
function Building(levels, width, columns)
    local t = {
        "n_building",
        {
            "c_buildingFloor",
            mat=buildingFloor,
            tra={-0.05, 0, -0.05},
            sca={width+0.1, 2, width+0.1},
        },
        {
            "c_buildingGlass",
            mat=buildingGlass,
            sca={windowThickness, 10*levels, width},
        },
        {
            "c_buildingGlass",
            mat=buildingGlass,
            sca={width, 10*levels, windowThickness},
        },
        {
            "c_buildingGlass",
            mat=buildingGlass,
            tra={width-windowThickness, 0, 0},
            sca={windowThickness, 10*levels, width},
        },
        {
            "c_buildingGlass",
            mat=buildingGlass,
            tra={0, 0, width-windowThickness},
            sca={width, 10*levels, windowThickness},
        },
        BuildingFacade(levels, width, columns),
        BuildingFacade(levels, width, columns, {0, 0, width}, {y, 90}),
        -- BuildingFacade(levels, width, columns, {width, 0, width}, {y, 180}),
        -- BuildingFacade(levels, width, columns, {width, 0, 0}, {y, 270}),
    }
    for level = 0, levels-1 do
        table.insert(t, {
            "n_buildingLevel",
            tra={0, 2+level*10, 0},
            {
                "c_buildingFloor",
                mat=buildingFloor,
                tra={-0.05, 8, -0.05},
                sca={width+0.1, 2, width+0.1},
            },
            -- {
            --     "c_buildingGlass",
            --     mat=buildingGlass,
            --     sca={windowThickness, 8, width},
            -- },
            -- {
            --     "c_buildingGlass",
            --     mat=buildingGlass,
            --     sca={width, 8, windowThickness},
            -- },
            -- {
            --     "c_buildingGlass",
            --     mat=buildingGlass,
            --     tra={width-windowThickness, 0, 0},
            --     sca={windowThickness, 8, width},
            -- },
            -- {
            --     "c_buildingGlass",
            --     mat=buildingGlass,
            --     tra={0, 0, width-windowThickness},
            --     sca={width, 8, windowThickness},
            -- },
        })
    end
    return t
end

function BoxBuilding(levels, width, columns)
    return {
        "c_boxBuilding",
        mat=buildingFloor,
        sca={width, levels*10, width},
    }
end

function CylinderBuilding(levels, width, columns)
    local hwidth = width/2
    local t = {
        "n_cylinderBuilding",
        tra={hwidth, 0, hwidth},
        {
            "m_buildingFloor",
            mesh="cylinder.obj",
            mat=buildingFloor,
            tra={0, 1, 0},
            sca={hwidth+0.1, 1, hwidth+0.1},
        },
        {
            "m_buildingGlass",
            mesh="cylinder.obj",
            mat=buildingGlass,
            tra={0, 10*levels/2, 0},
            sca={hwidth, 10*levels/2, hwidth},
        },
        {
            "n_facade_tra",
            tra={-hwidth, 0, -hwidth},
            BuildingFacade(levels, width, columns),
            BuildingFacade(levels, width, columns, {0, 0, width}, {y, 90}),
            -- BuildingFacade(levels, width, columns, {width, 0, width}, {y, 180}),
            -- BuildingFacade(levels, width, columns, {width, 0, 0}, {y, 270}),
        }
    }
    for level = 0, levels-1 do
        table.insert(t, {
            "n_buildingLevel",
            tra={0, 2+level*10, 0},
            {
                "m_buildingFloor",
                mesh="cylinder.obj",
                mat=buildingFloor,
                tra={0, 9, 0},
                sca={hwidth+0.1, 1, hwidth+0.1},
            },
            -- {
            --     "c_buildingGlass",
            --     mat=buildingGlass,
            --     sca={windowThickness, 8, width},
            -- },
            -- {
            --     "c_buildingGlass",
            --     mat=buildingGlass,
            --     sca={width, 8, windowThickness},
            -- },
            -- {
            --     "c_buildingGlass",
            --     mat=buildingGlass,
            --     tra={width-windowThickness, 0, 0},
            --     sca={windowThickness, 8, width},
            -- },
            -- {
            --     "c_buildingGlass",
            --     mat=buildingGlass,
            --     tra={0, 0, width-windowThickness},
            --     sca={width, 8, windowThickness},
            -- },
        })
    end
    return t
end


root = {
    "n_root",
    {
        "m_earth",
        mat=grass,
        mesh="plane.obj",
        mat=grass,
        sca=10000,
    },
    {
        "n_scene",
        tra={0, 0, 0},
        rot={y, -10},
        -- {
        --     "m_ground",
        --     mesh="plane.obj",
        --     mat=grass,
        --     sca=100,
        -- },
        {
            "c_road1",
            mat=road,
            tra={-40, 0.25, -1000}, -- x -40 to -20
            sca={20, 0.25, 1200},
        },
        {
            "c_road2",
            mat=road,
            tra={-200, 0.25, 80}, -- z 80 to 100
            sca={400, 0.25, 20},
        },
        {
            "c_median11",
            mat=median,
            tra={-30.5, 0.5, -1000},
            sca={1, 0.1, 1080}
        },
        {
            "c_median12",
            mat=median,
            tra={-30.5, 0.5, 100},
            sca={1, 0.1, 100}
        },
        {
            "c_median21",
            mat=median,
            tra={-200, 0.5, 89.5},
            sca={160, 0.1, 1}
        },
        {
            "c_median22",
            mat=median,
            tra={-20, 0.5, 89.5},
            sca={220, 0.1, 1}
        },
        {
            "n_building_f1",
            tra={-5, 0, 15},
            Building(8, 50, {0.25, 0.75}),
        },
        {
            "n_building_f2",
            tra={90, 0, 15},
            {
                "n_building_f2_2",
                rot={y, 20},
                CylinderBuilding(12, 50, {0.5}),
            }
        },
        {
            "n_building_r0",
            tra={-10, 0, -120},
            Building(2, 50, {0.2, 0.4, 0.6, 0.8}),
        },
        {
            "n_building_r1",
            tra={0, 0, -200},
            Building(8, 30, {0.25, 0.75}),
        },
        {
            "n_building_r2",
            tra={-10, 0, -400},
            Building(11, 30, {0.25, 0.75}),
        },
        {
            "n_building_l1",
            tra={-150, 0, -180},
            Building(2, 80, {0.2, 0.4, 0.6, 0.8}),
        },
        {
            "n_building_l2",
            tra={-110, 0, -500},
            Building(5, 30, {0.25, 0.75}),
        },
        {
            "n_building_l3",
            tra={-90, 0, -800},
            Building(7, 30, {0.25, 0.75}),
        },
        {
            "n_building_r3",    
            tra={-10, 0, -800},
            Building(7, 30, {0.25, 0.75}),
        },
        {
            "n_statue",
            tra={-5, 0, 115},
            {
                "s_statue",
                mat=statue,
                tra={0, 10, 0},
                sca={5, 10, 5},
            },
            {
                "c_statueBase",
                mat=concrete,
                tra={-6, 0, -6},
                sca={12, 1, 12},
            },
        },
        -- {
        --     "s_water",
        --     mat=water,
        --     tra={-50, 12, 50},
        --     sca=12,
        -- },
        {
            "n_pool",
            tra={-105, -1, 10},
            sca={5, 2, 5},
            {
                "c_poolWall",
                mat=concrete,
                tra={-10.5, 0, -10.5},
                sca={21, 2.25, 0.5},
            },
            {
                "c_poolWall",
                mat=concrete,
                tra={-10.5, 0, 10},
                sca={21, 2.25, 0.5},
            },
            {
                "c_poolWall",
                mat=concrete,
                tra={-10.5, 0, -10},
                sca={0.5, 2.25, 20},
            },
            {
                "c_poolWall",
                mat=concrete,
                tra={10, 0, -10},
                sca={0.5, 2.25, 20},
            },
            {
                "m_water",
                mesh="smooth_water_volume.obj",
                mat=water,
                tra={0, 1, 0},
            },
        },
        {
            "c_station",
            mat=concrete,
            tra={-20, 0.25, -45},
            sca={12, 2, 90},
        },
        {
            "n_train",
            tra={-25, 0.5, 200},
            {
                "k_trainArrive",
                t0=16.8,
                t1=17.2,
                ease=easeOutQuad,
                tra={0, 0, -200},
                {
                    "k_trainLeave",
                    t0=17.4,
                    t1=18.2,
                    ease=easeInQuad,
                    tra={0, 0, -500},
                    {
                        "n_train",
                        sca=5,  
                        {
                            "m_train",
                            mesh="smooth_train.obj",
                            mat=train,
                        },
                        {
                            "m_trainGlass",
                            mesh="smooth_train_glass.obj",
                            mat=trainGlass,
                        },
                    },
                },
            },
        },
    }
}

scene = parseNode(root)

-- Lights

white_light = gr.light({-400.0, 200.0, -400.0}, {0.9, 0.9, 0.9}, {1, 0, 0})
-- magenta_light = gr.light({400.0, 100.0, 150.0}, {0.7, 0.0, 0.7}, {1, 0, 0})

fps = 30
gr.animate(scene, 'final/%d.png', 1024, 1024,
    17*fps, 16.5, 0.1/fps,  -- numFrames, startTime, timeIncrement
-- gr.render(scene, 'final.png', 512, 512,
    -- 17.5,  -- timeOfDay
    {-80, 50, 160}, {0.5, 0, -1}, {0, 1, 0}, 90,
	{0.3, 0.3, 0.3}, {},
    150, 0)  -- focalDistance, blurAmount
