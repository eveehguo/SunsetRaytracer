require "common"

-- gr.material({kd}, {ks}, shininess, reflectivity, transparency, refractiveIndex, glossiness)

grass = gr.material({0.3, 0.6, 0.3}, {0, 0, 0}, 0)

box = gr.material({0.85, 0.8, 0.75}, {0.5, 0.5, 0.5}, 25)


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
		{
            "s_ball",
            mat=box,
            tra={-3, 2, 0},
            sca=2,
        },
        {
            "c_box",
            mat=box,
            tra={3-2, 0, -2},
            rot={y,30},
            sca=4,
        }
    }
}

scene = parseNode(root)

-- Lights

gr.render(scene, 'obj4.png', 512, 512,
	17,
    {7, 3, -7}, {-1, -0.2, 1}, {0, 1, 0}, 75,
	{0.3, 0.3, 0.3}, {})
