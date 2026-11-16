# 2026-11-16T13:03:25.570284400
import vitis

client = vitis.create_client()
client.set_workspace(path="D:/soc_project_vitis")

platform = client.get_component(name="platform")
status = platform.build()

comp = client.get_component(name="soc_component")
comp.build()

status = platform.build()

status = platform.build()

comp.build()

vitis.dispose()

