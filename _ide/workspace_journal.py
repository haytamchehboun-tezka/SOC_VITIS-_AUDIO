# 2026-11-16T14:22:32.260695400
import vitis

client = vitis.create_client()
client.set_workspace(path="D:/soc_project_vitis")

platform = client.get_component(name="platform")
status = platform.update_hw(hw_design = "D:\Soc_Project\audio_wrapper.xsa")

status = platform.update_hw(hw_design = "D:\Soc_Project\audio_wrapper.xsa")

status = platform.build()

comp = client.get_component(name="soc_component")
comp.build()

status = platform.build()

status = platform.build()

comp.build()

status = platform.build()

status = platform.build()

comp.build()

status = platform.build()

comp.build()

status = platform.build()

comp.build()

status = platform.build()

status = platform.build()

comp.build()

status = platform.build()

comp.build()

status = platform.build()

comp.build()

status = platform.build()

comp.build()

status = platform.build()

comp.build()

status = platform.build()

status = platform.build()

comp.build()

status = platform.build()

comp.build()

status = platform.build()

comp.build()

status = platform.build()

comp.build()

