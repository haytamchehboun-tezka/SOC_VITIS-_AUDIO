# 2026-11-16T13:37:56.124289500
import vitis

client = vitis.create_client()
client.set_workspace(path="D:/soc_project_vitis")

platform = client.get_component(name="platform")
status = platform.update_hw(hw_design = "D:\Soc_Project\audio_wrapper.xsa")

vitis.dispose()

