#! /usr/bin/env python

from hermes2d import Mesh, MeshView
from hermes2d.examples import get_example_mesh
domain_mesh = get_example_mesh()

mesh = Mesh()
mesh.load(domain_mesh)
#mesh.refine_element(2)
#mesh.refine_all_elements()
#mesh.refine_all_elements()
#mesh.refine_all_elements()

mview = MeshView("Hello world!", 100, 100, 500, 500)
mview.show(mesh, lib="mpl", method="orders", notebook=False)
mview.wait()
