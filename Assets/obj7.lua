require "common"

-- gr.material({kd}, {ks}, shininess, reflectivity, transparency, refractiveIndex, glossiness)

grass = gr.material({0.3, 0.6, 0.3}, {0, 0, 0}, 0)

box = gr.material({0.85, 0.8, 0.75}, {0.5, 0.5, 0.5}, 25)


cbox = {
    "c_box",
    mat=box,
    tra={-1, -1, -1},
    sca=2,
}

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
            "n_1",
            tra={0, 3, 0},
            {
                "k_1",
                t0=17,
                t1=17.03,
                ease=easeInQuad,
                tra={0, -2, 0},
                {
                    "k_2",
                    t0=17.05,
                    t1=17.08,
                    ease=easeInOutQuad,
                    tra={2, 0, 0},
                    rot={y, 90},
                    sca=2,
                    cbox,
                }
            }
        }
    }
}

scene = parseNode(root)

-- Lights

gr.animate(scene, 'obj7/%d.png', 512, 512,
	20, 17, 0.005, -- 17 to 17.09
    {7, 3, -7}, {-1, -0.2, 1}, {0, 1, 0}, 75,
	{0.3, 0.3, 0.3}, {})
