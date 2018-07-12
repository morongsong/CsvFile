


local TheTargetName='MCsvFile'
workspace (TheTargetName)
    configurations {'Debug', 'Release'}
    platforms {"Win32","x64"}
    --location ('_Build'..the_vs_name)
    characterset ('Unicode')
	
project (TheTargetName)
    targetname (TheTargetName)
    kind 'ConsoleApp'
    linkoptions {'/ENTRY:wmainCRTStartup'}

    files {"*.h","*.cpp","*.lua"}

    --pchheader "stdafx.h"
    --pchsource "stdafx.cpp"

