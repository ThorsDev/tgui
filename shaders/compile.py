import os
import subprocess

SHADERC_PATH = "shaderc.exe"
INPUT_PATH = "src"
OUTPUT_PATH = "out"

BIN2C_ENABLED = True

SHADERS = [
	"quad",
]

PROFILES = [
	{ "name" : "dx11", "platform" : "windows", "profile" : "s_5_0" },
	{ "name" : "dxil", "platform" : "windows", "profile" : "s_6_9" },
	{ "name" : "spv", "platform" : "linux", "profile" : "spirv16-13" },
	{ "name" : "glsl", "platform" : "linux", "profile" : "440" },
	{ "name" : "essl", "platform" : "linux", "profile" : "320_es" },
	{ "name" : "wgsl", "platform" : "linux", "profile" : "wgsl" },
	{ "name" : "mtl", "platform" : "osx", "profile" : "metal31-14" },
]

def CompileVertexShader(shader, profile):
	args = []
	
	## input
	args.append("-f")
	args.append(f"{INPUT_PATH}/{shader}_vs.sc")
	
	## output
	args.append("-o")
	args.append(f"{OUTPUT_PATH}/{shader}_{profile["name"]}_vs.bin")
	
	## profile
	if "profile" in profile:
		args.append("-p")
		args.append(f"{profile["profile"]}")
	
	## type
	args.append("--type")
	args.append("vertex")
	
	## platform
	args.append("--platform")
	args.append(f"{profile["platform"]}")
	
	## varying
	args.append("--varyingdef")
	args.append(f"{INPUT_PATH}/{shader}_varying.def.sc")
	
	## bin2c
	if True == BIN2C_ENABLED:
		args.append("--bin2c")
		args.append(f"{shader}_vs_{profile["name"]}")
		
	subprocess.run([SHADERC_PATH, *args], check=False)
		
def CompileFragmentShader(shader, profile):
	args = []
	
	## input
	args.append("-f")
	args.append(f"{INPUT_PATH}/{shader}_fs.sc")
	
	## output
	args.append("-o")
	args.append(f"{OUTPUT_PATH}/{shader}_{profile["name"]}_fs.bin")
	
	## profile
	if "profile" in profile:
		args.append("-p")
		args.append(f"{profile["profile"]}")
	
	## type
	args.append("--type")
	args.append("fragment")
	
	## platform
	args.append("--platform")
	args.append(f"{profile["platform"]}")
	
	## varying
	args.append("--varyingdef")
	args.append(f"{INPUT_PATH}/{shader}_varying.def.sc")
	
	## bin2c
	if True == BIN2C_ENABLED:
		args.append("--bin2c")
		args.append(f"{shader}_fs_{profile["name"]}")
		
	subprocess.run([SHADERC_PATH, *args], check=False)
		
def CompileShaders():
	if not os.path.exists(OUTPUT_PATH):
		os.makedirs(OUTPUT_PATH)
		
	for shader in SHADERS:
		for profile in PROFILES:
			CompileVertexShader(shader, profile)
			CompileFragmentShader(shader, profile)
			
	if True == BIN2C_ENABLED:
		for shader in SHADERS:
			with open(f"{OUTPUT_PATH}/{shader}_vs.bin.h", "w", encoding="utf-8") as f:
				
				for profile in PROFILES:
					filePath = f"{OUTPUT_PATH}/{shader}_{profile["name"]}_vs.bin"
					with open(filePath, "r", encoding="utf-8") as rf:
						data = rf.read()
						f.write(data)
						f.write("\n")
						
					if os.path.exists(filePath):
						os.remove(filePath)
						
			with open(f"{OUTPUT_PATH}/{shader}_fs.bin.h", "w", encoding="utf-8") as f:
				
				for profile in PROFILES:
					filePath = f"{OUTPUT_PATH}/{shader}_{profile["name"]}_fs.bin"
					with open(filePath, "r", encoding="utf-8") as rf:
						data = rf.read()
						f.write(data)
						f.write("\n")
						
					if os.path.exists(filePath):
						os.remove(filePath)
					
if "__main__" == __name__:
	CompileShaders()