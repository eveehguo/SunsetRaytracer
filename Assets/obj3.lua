require "common"

-- gr.material({kd}, {ks}, shininess, reflectivity, transparency, refractiveIndex, glossiness)

grass = gr.material({0.3, 0.6, 0.3}, {0, 0, 0}, 0)

box = gr.material({0.85, 0.8, 0.75}, {0.5, 0.5, 0.5}, 25)


c_box = {
    "c_box",
    mat=box,
    sca={1, 2, 1},
    tra={-1, 0, -10},
}

root = {
    "n_root",
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
		sca=100,
	},
    {
        "n_box",
        tra={-5, 0, 0},
        c_box,
    },
    {
        "n_box",
        tra={-2.5, 0, 0},
        c_box,
    },
    {
        "n_box",
        tra={0, 0, 0},
        c_box,
    },
    {
        "n_box",
        tra={2.5, 0, 0},
        c_box,
    },
    {
        "n_box",
        tra={5, 0, 0},
        c_box,
    },
    {
        "n_scene",
		tra={0, 1, 0},
		{
			"s_1",
			mat=gr.material({0.7, 0.7, 0.7}, {0, 0, 0}, 0, 1.0, 0.0, 1.33, 0.05),
			tra={-5, 0, 0},
		},
		{
			"s_2",
			mat=gr.material({0.7, 0.7, 0.7}, {0, 0, 0}, 0, 0.5, 0.5, 1.33, 0.05),
			tra={-2.5, 0, 0},
		},
		{
			"s_3",
			mat=gr.material({0.7, 0.7, 0.7}, {0, 0, 0}, 0, 0.5, 1.0, 1.33, 0.05),
			tra={0, 0, 0},
		},
		{
			"s_4",
			mat=gr.material({0.7, 0.7, 0.7}, {0, 0, 0}, 0, 0.5, 1.0, 1.5, 0.05),
			tra={2.5, 0, 0},
		},
		{
			"s_5",
			mat=gr.material({0.7, 0.7, 0.7}, {0, 0, 0}, 0, 0.5, 1.0, 1.7, 0.05),
			tra={5, 0, 0},
		},
    }
}

scene = parseNode(root)

-- Lights

gr.render(scene, 'obj3.png', 512, 512,
    17,
    {0, 3, 10}, {0, -0.2, -1}, {0, 1, 0}, 75,
	{0.3, 0.3, 0.3}, {})
