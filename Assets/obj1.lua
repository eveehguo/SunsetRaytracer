require "common"

-- gr.material({kd}, {ks}, shininess, reflectivity, transparency, refractiveIndex, glossiness)

grass = gr.material({0.3, 0.6, 0.3}, {0, 0, 0}, 0)


root = {
    "n_root",
	rot={y, 135},
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
        "n_scene",
		tra={0, 1, 0},
		{
			"s_1",
			mat=gr.material({0.7, 0.7, 0.7}, {0, 0, 0}, 0, 0.0),
			tra={-5, 0, 0},
		},
		{
			"s_2",
			mat=gr.material({0.7, 0.7, 0.7}, {0, 0, 0}, 0, 0.25),
			tra={-2.5, 0, 0},
		},
		{
			"s_3",
			mat=gr.material({0.7, 0.7, 0.7}, {0, 0, 0}, 0, 0.5),
			tra={0, 0, 0},
		},
		{
			"s_4",
			mat=gr.material({0.7, 0.7, 0.7}, {0, 0, 0}, 0, 0.75),
			tra={2.5, 0, 0},
		},
		{
			"s_5",
			mat=gr.material({0.7, 0.7, 0.7}, {0, 0, 0}, 0, 1.0),
			tra={5, 0, 0},
		},
    }
}

scene = parseNode(root)

-- Lights

gr.render(scene, 'obj1.png', 512, 512,
	17,
    {7, 3, -7}, {-1, -0.2, 1}, {0, 1, 0}, 75,
	{0.3, 0.3, 0.3}, {})
