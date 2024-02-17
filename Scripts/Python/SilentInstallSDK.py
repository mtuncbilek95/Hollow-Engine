import subprocess
import sys

print("\033[32m WELCOME TO PORTAKAL ENGINE\n-----------------------------\n\033[00m")

def install(package):
	subprocess.check_call([sys.executable, "-m", "pip", "install", package])

try:
	import os
except ImportError:
	print("os module is not installed. Installing...")
	install(os)

import os

try:
	import requests
except ImportError:
	print("requests module is not installed. Installing...")
	install(requests)

import requests

try:
	import glob
except ImportError:
	print("glob module is not installed. Installing...")
	install(glob)

import glob

try:
	import shutil
except ImportError:
	print("shutil module is not installed. Installing...")
	install(shutil)

import shutil

try:
	import time
except ImportError:
	print("time module is not installed. Installing...")
	install(time)

import time

from colorama import Fore, Style

sleepTime = 0.2

print(Fore.CYAN + "[INFO] -- " + Style.RESET_ALL + "All the python libraries are up to date.")
time.sleep(sleepTime)
print(Fore.CYAN + "[INFO] -- " + Style.RESET_ALL + "Checking if VulkanSDK is up to date...")
time.sleep(sleepTime*2)

ourVersion = "1.3.268.0"

sdkPath = "C:/VulkanSDK/"
# Get a initList of all files and directories in the directory
allItems = os.listdir(sdkPath)

# Filter the initList to only include directories
dirs = [item for item in allItems if os.path.isdir(os.path.join(sdkPath, item))]

# Convert the directory names to tuples of integers
dirsTuple = [tuple(map(int, sdkPath.split('.'))) for sdkPath in dirs]

# Check the tuple length
if len(dirsTuple) == 0:
	print(Fore.RED + "[WARNING] -- " + Style.RESET_ALL + "No version of Vulkan SDK found")
	exit()

# Find the maximum version number
maxVersion = max(dirsTuple)

# Convert the maximum version number back to a string
maxVersionStr = '.'.join(map(str, maxVersion))

# Check if maxVersionStr is same with ourVersion 
if maxVersionStr == ourVersion:
	print(Fore.GREEN + "[SUCCESS] -- " + Style.RESET_ALL + "VulkanSDK is up to date")
	time.sleep(sleepTime * 3)
# Else print error and exit in 3 seconds
else:
	print(Fore.RED + "[WARNING] -- " + Style.RESET_ALL + "VulkanSDK is not up to date." + "Please update your VulkanSDK to version " + ourVersion)
	time.sleep(sleepTime * 3)
	exit()

print(Fore.CYAN + "[INFO] -- " + Style.RESET_ALL + "Copying necessary Shader Compilers to External folder")

sdkPath += maxVersionStr

# Save the path of this python script
externalPath = os.path.dirname(os.path.realpath(__file__))

# Go one folder back
externalPath = os.path.dirname(externalPath)
externalPath = os.path.dirname(externalPath)

# Create folder paths if there is no path 
if not os.path.exists(externalPath + "/External/Dxc/Include"):
	os.makedirs(externalPath + "/External/Dxc/Include")

if not os.path.exists(externalPath + "/External/Glslang/Include"):
	os.makedirs(externalPath + "/External/Glslang/Include")

if not os.path.exists(externalPath + "/External/SPIRV-Cross/Include"):
	os.makedirs(externalPath + "/External/SPIRV-Cross/Include")

if not os.path.exists(externalPath + "/External/Shaderc/Include"):
	os.makedirs(externalPath + "/External/Shaderc/Include")

if not os.path.exists(externalPath + "/External/Vulkan/Include"):
	os.makedirs(externalPath + "/External/Vulkan/Include")

if not os.path.exists(externalPath + "/External/Vulkan-Video/Include"):
	os.makedirs(externalPath + "/External/Vulkan-Video/Include")

if not os.path.exists(externalPath + "/External/Libs"):
	os.makedirs(externalPath + "/External/Libs")

# Copy files from sdkpath/Include/vulkan to externalPath/External/Vulkan/Include
for file in glob.glob(sdkPath + "/Include/vulkan/*.h"):
	shutil.copy(file, externalPath + "/External/Vulkan/Include")
	# Print just the file name without path
	print(Fore.GREEN + "[SUCCESS] -- " + Style.RESET_ALL + os.path.basename(file) + " has been copied to " + externalPath + "/External/Vulkan/Include/")
	time.sleep(sleepTime)

for file in glob.glob (sdkPath + "/Include/vulkan/*.hpp"):
	shutil.copy(file, externalPath + "/External/Vulkan/Include")
	# Print just the file name without path
	print(Fore.GREEN + "[SUCCESS] -- " + Style.RESET_ALL + os.path.basename(file) + " has been copied to " + externalPath + "/External/Vulkan/Include/")
	time.sleep(sleepTime)

# Copy files from sdkpath/Include/vulkan_video to externalPath/External/Vulkan-Video/Include
for file in glob.glob(sdkPath + "/Include/vk_video/*.h"):
	shutil.copy(file, externalPath + "/External/Vulkan-Video/Include")
	# Print just the file name without path
	print(Fore.GREEN + "[SUCCESS] -- " + Style.RESET_ALL + os.path.basename(file) + " has been copied to " + externalPath + "/External/Vulkan-Video/Include/")
	time.sleep(sleepTime)

# Copy files from sdkpath/Include/dxc to externalPath/External/Dxc/Include
for file in glob.glob(sdkPath + "/Include/dxc/*.h"):
	shutil.copy(file, externalPath + "/External/Dxc/Include")
	# Print just the file name without path
	print(Fore.GREEN + "[SUCCESS] -- " + Style.RESET_ALL + os.path.basename(file) + " has been copied to " + externalPath + "/External/Dxc/Include/")
	time.sleep(sleepTime)

# Copy files from sdkpath/Include/glslang/Include to externalPath/External/Glslang/Include
for file in glob.glob(sdkPath + "/Include/glslang/Include/*.h"):
	shutil.copy(file, externalPath + "/External/Glslang/Include")
	print(Fore.GREEN + "[SUCCESS] -- " + Style.RESET_ALL + os.path.basename(file) + " has been copied to " + externalPath + "/External/Glslang/Include/")
	time.sleep(sleepTime)

# Copy files from sdkpath/Include/spirv_cross to externalPath/External/SPIRV-Cross/Include
for file in glob.glob(sdkPath + "/Include/spirv_cross/*.hpp"):
	shutil.copy(file, externalPath + "/External/SPIRV-Cross/Include")
	print(Fore.GREEN + "[SUCCESS] -- " + Style.RESET_ALL + os.path.basename(file) + " has been copied to " + externalPath + "/External/SPIRV-Cross/Include/")
	time.sleep(sleepTime)

for file in glob.glob(sdkPath + "/Include/spirv_cross/*.h"):
	shutil.copy(file, externalPath + "/External/SPIRV-Cross/Include")
	print(Fore.GREEN + "[SUCCESS] -- " + Style.RESET_ALL + os.path.basename(file) + " has been copied to " + externalPath + "/External/SPIRV-Cross/Include/")
	time.sleep(sleepTime)

# Copy files from sdkpath/Include/shaderc to externalPath/External/Shaderc/Include
for file in glob.glob(sdkPath + "/Include/shaderc/*.h"):
	shutil.copy(file, externalPath + "/External/Shaderc/Include")
	print(Fore.GREEN + "[SUCCESS] -- " + Style.RESET_ALL + os.path.basename(file) + " has been copied to " + externalPath + "/External/Shaderc/Include/")
	time.sleep(sleepTime)

for file in glob.glob(sdkPath + "/Include/shaderc/*.hpp"):
	shutil.copy(file, externalPath + "/External/Shaderc/Include")
	print(Fore.GREEN + "[SUCCESS] -- " + Style.RESET_ALL + os.path.basename(file) + " has been copied to " + externalPath + "/External/Shaderc/Include/")
	time.sleep(sleepTime)

# Copy files from sdkpath/Lib to externalPath/External/Libs
for file in glob.glob(sdkPath + "/Lib/*.lib"):
	shutil.copy(file, externalPath + "/External/Libs/")
	print(Fore.GREEN + "[SUCCESS] -- " + Style.RESET_ALL + os.path.basename(file) + " has been copied to " + externalPath + "/External/Libs/")
	time.sleep(sleepTime)



print(Fore.GREEN + "[SUCCESS] -- " + Style.RESET_ALL + "All necessary Shader Compilers were copied")

print(Fore.CYAN + "[INFO] -- " + Style.RESET_ALL + "Editing the header files according to our system")

with open(externalPath + "/External/Shaderc/Include/shaderc.h", "r") as file:
  lines = file.readlines()

for i in [25, 26, 27]:
  lines[i] = lines[i].replace('shaderc/', '')

with open(externalPath + "/External/Shaderc/Include/shaderc.h", "w") as file:
  file.writelines(lines)

with open(externalPath + "/External/Vulkan/Include/vulkan_core.h", "r") as file:
  lines = file.readlines()

for i in [8373, 8374, 8375, 9589, 9590, 9591]:
  lines[i] = lines[i].replace('vk_video/', '../../Vulkan-Video/Include/')

with open(externalPath + "/External/Vulkan/Include/vulkan_core.h", "w") as file:
  file.writelines(lines)

print(Fore.GREEN + "[SUCCESS] -- " + Style.RESET_ALL + "Needed files has been edited.")

# Build ReflectionParserTool 
print(Fore.CYAN + "[INFO] -- " + Style.RESET_ALL + "Building ReflectionParserTool at " + externalPath + "Tools/ReflectionParser/ReflectionParser.csproj")

subprocess.run(["dotnet", "build", externalPath + "/Tools/ReflectionParser/ReflectionParser.csproj"])

print(Fore.GREEN + "[SUCCESS] -- " + Style.RESET_ALL + "ReflectionParserTool has been built.")

print(Fore.YELLOW + "[WARNING] -- " + Style.RESET_ALL + "If you still get compiler error during the build, please check the Portakal/wiki/Home from github.com")
time.sleep(sleepTime)
exit()