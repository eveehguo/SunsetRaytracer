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
}

scene = parseNode(root)

-- Lights

gr.animate(scene, 'obj6/%d.png', 512, 512,
    8, 12, 1,
    {-80, 50, 160}, {0.5, 0.5, -1}, {0, 1, 0}, 90,
	{0.3, 0.3, 0.3}, {})
