import bpy # type: ignore
import os
import math ,struct
import bpy_extras # type: ignore

log_file = 0

def log(string):
    settings = bpy.context.scene.export_settings

    global log_file
    if settings.logging:
        log_file.write(string)

COMPRESSED_NORMAL_POSITION = 10.0

def export_mesh(mesh, file):
    settings = bpy.context.scene.export_settings

    #write vertex count
    total_triangles = len(mesh.loop_triangles)
    file.write(struct.pack('<i', total_triangles*3))
    log("Number of triangles: " + str(total_triangles) + "\n")
    log("Number of vertices: " + str(len(mesh.vertices)) + "\n")
    log("Number of vertex colors: " + str(len(mesh.vertex_colors.active.data)) + "\n")
    log("\n")

    triangle_number = 0
    while triangle_number < total_triangles:
        triangle = mesh.loop_triangles[triangle_number]
        
        triangle_vertex = 0
        while triangle_vertex < 3:
            loop_index = triangle.loops[triangle_vertex]
            index = triangle.vertices[triangle_vertex]
            vert = mesh.vertices[index].co
            file.write(struct.pack('<fff', vert.x, vert.y, vert.z))
            log(str(index) + "\tPosition: %.5f, %.5f, %.5f \n" % (vert.x, vert.y, vert.z))

            if settings.export_colors:
                color = mesh.vertex_colors.active.data[loop_index].color
                file.write(struct.pack('<fff', color[0], color[1], color[2]))
                log("\t\tVertex color: %.5f, %.5f, %.5f \n" % (color[0], color[1], color[2]))
            if settings.export_uvs:
                coords = mesh.uv_layers.active.data[loop_index].uv
                file.write(struct.pack('<ff', coords[0], coords[1]))
                log("\t\tUV coordinates: %.3f, %.3f \n" % (coords[0], coords[1]))
            triangle_vertex += 1

        triangle_number += 1
        log("----------------------------------------------------------------\n")

def export_scene(filepath):
    settings = bpy.context.scene.export_settings
    
    global log_file
    if settings.logging:
        log_file = open(bpy.data.filepath.replace(".blend", "_log.txt"),"w")

    file = open(filepath, "wb")
    log(filepath + "\n")

    for ob in bpy.context.visible_objects:
        if ob.type == 'MESH':#Only export meshes
            mesh = ob.to_mesh() #maybe use ob.data?

            if settings.apply_transforms:
                mesh.transform(ob.matrix_world)

            # if settings.apply_modifiers:
            #     for _, m in enumerate(ob.modifiers):
            #         # LOOPOBS['modifier'] = m
            #         bpy.ops.object.modifier_apply(LOOPOBS, modifier=m.name)

            export_mesh(mesh, file)
            
	#End Selection Loop

    file.flush()
    file.close()

    if settings.logging:
        log_file.flush()
        log_file.close()

#------------------------------------
bl_info = {
    "name": "Export to custom binary format",
    "blender": (2, 80, 0),
    "category": "Import/Export",
    "description": "Exports custom file format"
}

class Settings(bpy.types.PropertyGroup):
    apply_modifiers : bpy.props.BoolProperty(name = "Apply Modifiers", default = False) # type: ignore
    apply_transforms : bpy.props.BoolProperty(name = "Apply Transforms", default = True) # type: ignore
    export_normals : bpy.props.BoolProperty(name = "Export Normals", default = False) # type: ignore
    normal_type : bpy.props.EnumProperty(name = "Type",
                                            items = [('uncompressed', "Uncompressed normals", ""),
                                                    ('compressed', "Compressed normals", "")]) # type: ignore
    export_colors : bpy.props.BoolProperty(name = "Export Vertex Colors", default = True) # type: ignore
    export_uvs : bpy.props.BoolProperty(name = "Export Texture Coordinates", default = False) # type: ignore
    uv_type : bpy.props.EnumProperty(name = "Type",
                                        items = [('flipped', "Flipped Y coordinate", ""),
                                                ('unflipped', "Normal coordinates", "")]) # type: ignore
    logging : bpy.props.BoolProperty(name = "Export debug text", default = False) # type: ignore


class ObjectExport(bpy.types.Operator, bpy_extras.io_utils.ImportHelper):
    """Export visible objects in custom format"""      # Use this as a tooltip for menu items and buttons.
    bl_idname = "object.export_custom"        # Unique identifier for buttons and menu items to reference.
    bl_label = "Export to custom binary format"         # Display name in the interface.
    bl_options = {'REGISTER'}

    def execute(self, context):

        export_scene(self.filepath)

        return {'FINISHED'}            # Lets Blender know the operator finished successfully.

class ExportPanel(bpy.types.Panel):
    bl_space_type = "VIEW_3D"
    bl_region_type = "UI"
    bl_label = "Custom export settings"
    bl_category = "Custom export"

    def draw(self, context):
        settings = bpy.context.scene.export_settings
#        self.layout.prop(settings, "apply_modifiers")
        self.layout.prop(settings, "apply_transforms")
#        self.layout.prop(settings, "export_normals")
#        self.layout.prop(settings, "normal_type")
        self.layout.prop(settings, "export_colors")
        self.layout.prop(settings, "export_uvs")
        self.layout.prop(settings, "uv_type")
        self.layout.prop(settings, "logging")
        self.layout.row().operator(ObjectExport.bl_idname,text = "Export visible objects")

classes = [Settings, ObjectExport, ExportPanel]

def register():
    for cls in classes:
        bpy.utils.register_class(cls)
    bpy.types.Scene.export_settings = bpy.props.PointerProperty(type = Settings)

def unregister():
    for cls in classes:
        bpy.utils.unregister_class(cls)
    del bpy.types.scene.export_settings

if __name__ == "__main__":
    unregister()
    register()