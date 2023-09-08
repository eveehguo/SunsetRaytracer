require "common"

-- gr.material({kd}, {ks}, shininess, reflectivity, transparency, refractiveIndex, glossiness)

grass = gr.material({0.3, 0.6, 0.3}, {0, 0, 0}, 0)

box = gr.material({0.85, 0.8, 0.75}, {0.5, 0.5, 0.5}, 25)


c_box = {
    "c_box",
    mat=box,
    sca={50, 100, 50},
    tra={-50, 0, -50},
}

root = {
    "n_root",
    rot={y, -20},
    -- {
    --     "s_earth",
    --     mat=grass,
    --     tra={0, -6.36e6, 0},
    --     sca=6.36e6,
    -- },
	{
		"m_ground",
		mesh="plane.obj",
		mat=grass,
		sca=10000,
	},
    {
        "n_box1",
        tra={0, 0, 0},
        c_box,
    },
    {
        "n_box2",
        tra={0, 0, -200},
        c_box,
    },
    {
        "n_box3",
        tra={0, 0, -500},
        c_box,
    },
    {
        "n_box4",
        tra={0, 0, -1000},
        c_box,
    },
    {
        "n_box5",
        tra={0, 0, -2000},
        c_box,
    },
}

scene = parseNode(root)

-- Lights

gr.render(scene, 'obj5.png', 512, 512,
    17,
    {50, 140, 180}, {0, -0.2, -1}, {0, 1, 0}, 60,
	{0.2, 0.2, 0.2}, {},
    400, 0.005)
