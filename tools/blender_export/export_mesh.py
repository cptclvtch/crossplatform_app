import bpy # type: ignore
import os
import math ,struct
import bpy_extras # type: ignore

log_file = 0
input_folder = ""
output_folder = ""
total = 0
settings = 0

def log(string):
    global log_file
    settings = bpy.context.screen.export_settings
    if settings.logging:
        log_file.write(string)

COMPRESSED_NORMAL_POSITION = 10.0

def export_mesh(mesh, file):
    settings = bpy.context.screen.export_settings
    
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

            if settings.export_normals:
                normal = mesh.loops[loop_index].normal
                file.write(struct.pack('<fff', normal.x, normal.y, normal.z))
                log(str(index) + "\t\tNormal: %.5f, %.5f, %.5f \n" % (normal.x, normal.y, normal.z))

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
    global log_file

    settings = bpy.context.screen.export_settings
    if settings.logging:
        log_file = open(bpy.data.filepath.replace(".blend", "_log.txt"),"w")

    file = open(filepath, "wb")
    log(filepath + "\n")

    objects = bpy.context.view_layer.objects

    for ob in objects:
        if ob.type == 'MESH' and ob.visible_get():
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
    export_normals : bpy.props.BoolProperty(name = "Export Normals", default = True) # type: ignore
    normal_type : bpy.props.EnumProperty(name = "Type",
                                            items = [('uncompressed', "Uncompressed normals", ""),
                                                    ('compressed', "Compressed normals", "")]) # type: ignore
    export_colors : bpy.props.BoolProperty(name = "Export Vertex Colors", default = True) # type: ignore
    export_uvs : bpy.props.BoolProperty(name = "Export Texture Coordinates", default = False) # type: ignore
    uv_type : bpy.props.EnumProperty(name = "Type",
                                        items = [('flipped', "Flipped Y coordinate", ""),
                                                ('unflipped', "Normal coordinates", "")]) # type: ignore
    # force_export: bpy.props.BoolProperty(name = "Force export", default = False) #type: ignore
    logging : bpy.props.BoolProperty(name = "Export debug text", default = False) # type: ignore


class ObjectExport(bpy.types.Operator, bpy_extras.io_utils.ImportHelper):
    """Export visible objects in custom format"""      # Use this as a tooltip for menu items and buttons.
    bl_idname = "object.export_custom"        # Unique identifier for buttons and menu items to reference.
    bl_label = "Export to custom binary format"         # Display name in the interface.
    bl_options = {'REGISTER'}

    def execute(self, context):

        export_scene(self.filepath)

        return {'FINISHED'}            # Lets Blender know the operator finished successfully.

class FolderExport(bpy.types.Operator):
    """"Recursively export a folder"""
    bl_idname = "object.recursive_export"
    bl_label = "Recursively export a folder"
    bl_options = {'REGISTER'}

    def execute(self, context):
        global total, input_folder, output_folder, settings
        if os.path.exists(input_folder) and os.path.exists(output_folder):
            settings = bpy.context.screen.export_settings
            print("Force export: ", settings.force_export)
            total = 0
            #loop over every folder, except the "Deprecated" folder
            for subdir, dirs, files in os.walk(input_folder):

                if subdir.endswith('Deprecated'):
                    continue

                for file in files:
                    #compare "last modified" dates
                    
                    #do only .blends
                    if not file.endswith('.blend'):
                        continue

                    needs_update = False

                    equivalent_folder = subdir.replace(input_folder, output_folder)

                    if not os.path.exists(equivalent_folder):
                        os.makedirs(equivalent_folder)
                        needs_update = True

                    equivalent_path = os.path.join(equivalent_folder, file.replace('.blend','.mesh'))
                    original_path = os.path.join(subdir, file)

                    if not os.access(equivalent_path, os.F_OK):
                        needs_update = True
                    else:
                        original_mtime = os.path.getmtime(original_path)
                        equivalent_mtime = os.path.getmtime(equivalent_path)
                        if original_mtime > equivalent_mtime:
                            needs_update = True

                    if needs_update or settings.force_export:
                        #open file in blender
                        bpy.ops.wm.open_mainfile(filepath=original_path)

                        export_scene(equivalent_path)

                        # if not model_within_bounds:
                        #     os.remove(equivalent_path)
                        #     print("Model ",file," is out of bounds (coordinate value should be less than ", COMPRESSED_NORMAL_POSITION,"). Model removed. Please correct and re-run script.")

                        total += 1

            print("Total models updated: ", total)
        else:
            print("Input and output folders need to be set first")

        return {'FINISHED'}

class SetInputFolder(bpy.types.Operator, bpy_extras.io_utils.ImportHelper):
    """"Set Input Folder"""
    bl_idname = "object.input_folder"
    bl_label = "Set input folder"
    bl_options = {'REGISTER'}

    directory: bpy.props.StringProperty()

    filter_glob: bpy.props.StringProperty(default="", options={'HIDDEN'}, maxlen=255)

    def execute(self, context):
        global input_folder
        input_folder = self.directory
        print("Selected input folder: " + input_folder)
        return {'FINISHED'}

        
class SetOutputFolder(bpy.types.Operator, bpy_extras.io_utils.ImportHelper):
    """"Set Output Folder"""
    bl_idname = "object.output_folder"
    bl_label = "Set output folder"
    filename_ext = "."
    bl_options = {'REGISTER'}

    directory: bpy.props.StringProperty()

    filter_glob: bpy.props.StringProperty(default="", options={'HIDDEN'}, maxlen=255)

    def execute(self, context):
        global output_folder
        output_folder = self.directory
        print("Selected output folder:" + output_folder)
        return {'FINISHED'}

class ExportPanel(bpy.types.Panel):
    bl_space_type = "VIEW_3D"
    bl_region_type = "UI"
    bl_label = "Export"
    bl_category = "Custom Export"
    bl_idname = "OBJECT_PT_export"

    def draw(self, context):
        local_settings = bpy.context.screen.export_settings
#        self.layout.prop(local_settings, "apply_modifiers")
        self.layout.prop(local_settings, "apply_transforms")
        self.layout.prop(local_settings, "export_normals")
#        self.layout.prop(local_settings, "normal_type")
        self.layout.prop(local_settings, "export_colors")
        self.layout.prop(local_settings, "export_uvs")
        self.layout.prop(local_settings, "uv_type")
        self.layout.prop(local_settings, "logging")
        self.layout.row().operator(ObjectExport.bl_idname, text = "Export visible objects", icon = 'EXPORT')
        
        
class FolderExportPanel(bpy.types.Panel):
    bl_parent_id = "OBJECT_PT_export"
    bl_space_type = "VIEW_3D"
    bl_region_type = "UI"
    bl_label = "Recursive folder export"
    bl_category = "Custom export"
    bl_idname = "OBJECT_PT_folderexport"

    def draw(self, context):
        global total, input_folder, output_folder
        local_settings = bpy.context.screen.export_settings
        
        input_icon = 'FILEBROWSER'
        if os.path.exists(input_folder):
            input_icon = 'CHECKMARK'
        
        output_icon = 'FILEBROWSER'
        if os.path.exists(output_folder):
            output_icon = 'CHECKMARK'

        self.layout.row().operator(SetInputFolder.bl_idname, text = "Set input folder", icon = input_icon)
        self.layout.row().operator(SetOutputFolder.bl_idname, text = "Set output folder", icon = output_icon)
        self.layout.row().operator(FolderExport.bl_idname, text = "Recursive folder export", icon = 'EXPORT')
        # self.layout.row().prop(local_settings, "force_export")
        self.layout.row().label(text = "Assets exported: " + str(total))
        
classes = [Settings, ObjectExport, ExportPanel, FolderExportPanel, SetInputFolder, SetOutputFolder, FolderExport]

def register():
    for cls in classes:
        bpy.utils.register_class(cls)
    bpy.types.Screen.export_settings = bpy.props.PointerProperty(type = Settings)

def unregister():
    for cls in classes:
        bpy.utils.unregister_class(cls)

if __name__ == "__main__":
    unregister()
    register()