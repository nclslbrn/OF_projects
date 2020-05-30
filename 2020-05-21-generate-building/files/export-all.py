'''
BATCH EXPORT PLY FOR OF

Select every object/mesh you want to export in bin/data/plys
(ply filename created from object name)

'''

import bpy  # noqa
import os  # noqua


D = bpy.data
C = bpy.context
viewlayer = C.view_layer

path = os.path.dirname(os.path.dirname(D.filepath)) + '/bin/data/plys/'

obs = C.selected_objects

for object in obs:
    
    viewlayer.objects.active = object
    object.select_set(True)
    bpy.ops.export_mesh.ply(
        filepath=path + object.name + ".ply",
        check_existing=False,
        use_selection=True,
        use_mesh_modifiers=True,
        use_normals=True,
        use_colors=True,
        global_scale=1,
        axis_forward='Z',
        axis_up='Y'
    )
    bpy.ops.object.select_all(action='DESELECT')