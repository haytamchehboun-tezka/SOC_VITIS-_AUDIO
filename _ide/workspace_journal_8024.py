# 2026-11-14T20:38:18.075169100
import vitis

client = vitis.create_client()
client.set_workspace(path="D:/soc_project_vitis")

platform = client.create_platform_component(name = "platform",hw_design = "D:\Soc_Project\audio_wrapper.xsa",os = "standalone",cpu = "ps7_cortexa9_0",domain_name = "standalone_ps7_cortexa9_0",compiler = "gcc")

comp = client.create_app_component(name="soc_component",platform = "$COMPONENT_LOCATION/../platform/export/platform/platform.xpfm",domain = "standalone_ps7_cortexa9_0")

comp = client.get_component(name="soc_component")
status = comp.import_files(from_loc="", files=["D:\Soc_Project\Sources\C\audio_functions.c", "D:\Soc_Project\Sources\C\audio_functions.h", "D:\Soc_Project\Sources\C\main.c"], is_skip_copy_sources = False)

platform = client.get_component(name="platform")
status = platform.build()

status = platform.build()

comp = client.get_component(name="soc_component")
comp.build()

status = platform.build()

comp.build()

vitis.dispose()

