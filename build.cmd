@echo off
setlocal
cd /d "%~dp0"
call "C:\Program Files (x86)\Microsoft Visual Studio\18\BuildTools\Common7\Tools\VsDevCmd.bat" -arch=x64 -host_arch=x64 >nul
if errorlevel 1 exit /b 1
if not exist work\build mkdir work\build
cl /nologo /std:c++17 /EHsc /MT /W4 /I src tests\route_pattern_tests.cpp /Fo:work\build\route_pattern_tests.obj /Fe:work\build\route_pattern_tests.exe
if errorlevel 1 exit /b 1
work\build\route_pattern_tests.exe
if errorlevel 1 exit /b 1
cl /nologo /std:c++17 /EHsc /MT /W4 /I src tests\route_progress_tests.cpp /Fo:work\build\route_progress_tests.obj /Fe:work\build\route_progress_tests.exe
if errorlevel 1 exit /b 1
work\build\route_progress_tests.exe
if errorlevel 1 exit /b 1
cl /nologo /std:c++17 /EHsc /MT /W4 /I src tests\route_pulse_tests.cpp /Fo:work\build\route_pulse_tests.obj /Fe:work\build\route_pulse_tests.exe
if errorlevel 1 exit /b 1
work\build\route_pulse_tests.exe
if errorlevel 1 exit /b 1
cl /nologo /std:c++17 /EHsc /MT /W4 /I src tests\read_recovery_tests.cpp /Fo:work\build\read_recovery_tests.obj /Fe:work\build\read_recovery_tests.exe
if errorlevel 1 exit /b 1
work\build\read_recovery_tests.exe
if errorlevel 1 exit /b 1
cl /nologo /std:c++17 /EHsc /MT /O2 /W4 /I src tests\route_session_tests.cpp /Fo:work\build\route_session_tests.obj /Fe:work\build\route_session_tests.exe
if errorlevel 1 exit /b 1
work\build\route_session_tests.exe
if errorlevel 1 exit /b 1
cl /nologo /std:c++17 /EHsc /MT /W4 /I src tests\road_route_tests.cpp /Fo:work\build\road_route_tests.obj /Fe:work\build\road_route_tests.exe
if errorlevel 1 exit /b 1
work\build\road_route_tests.exe
if errorlevel 1 exit /b 1
cl /nologo /std:c++17 /EHsc /MT /W4 /I src tests\worldmap_roads_tests.cpp /Fo:work\build\worldmap_roads_tests.obj /Fe:work\build\worldmap_roads_tests.exe
if errorlevel 1 exit /b 1
work\build\worldmap_roads_tests.exe
if errorlevel 1 exit /b 1
cl /nologo /std:c++17 /EHsc /MT /O2 /W4 /I src tests\map_projection_tests.cpp /Fo:work\build\map_projection_tests.obj /Fe:work\build\map_projection_tests.exe
if errorlevel 1 exit /b 1
work\build\map_projection_tests.exe
if errorlevel 1 exit /b 1
cl /nologo /std:c++17 /EHsc /MT /O2 /W4 /I src tests\map_resolution_tests.cpp /Fo:work\build\map_resolution_tests.obj /Fe:work\build\map_resolution_tests.exe
if errorlevel 1 exit /b 1
work\build\map_resolution_tests.exe
if errorlevel 1 exit /b 1
if not exist outputs\local-test-1.0.7\build mkdir outputs\local-test-1.0.7\build
cl /nologo /std:c++17 /EHsc /MT /W4 /I src tests\settings_tests.cpp /Fo:work\build\settings_tests.obj /Fe:work\build\settings_tests.exe
if errorlevel 1 exit /b 1
work\build\settings_tests.exe
if errorlevel 1 exit /b 1
cl /nologo /std:c++17 /EHsc /MT /W4 /I src tests\map_reader_tests.cpp /Fo:work\build\map_reader_tests.obj /Fe:work\build\map_reader_tests.exe
if errorlevel 1 exit /b 1
work\build\map_reader_tests.exe
if errorlevel 1 exit /b 1
cl /nologo /std:c++17 /EHsc /MT /W4 /I src tests\target_reader_tests.cpp /Fo:work\build\target_reader_tests.obj /Fe:work\build\target_reader_tests.exe
if errorlevel 1 exit /b 1
work\build\target_reader_tests.exe
if errorlevel 1 exit /b 1
cl /nologo /std:c++17 /EHsc /MT /W4 /I src tests\minimap_geometry_tests.cpp /Fo:work\build\minimap_geometry_tests.obj /Fe:work\build\minimap_geometry_tests.exe
if errorlevel 1 exit /b 1
work\build\minimap_geometry_tests.exe
if errorlevel 1 exit /b 1
cl /nologo /std:c++17 /EHsc /MT /W4 /I src tests\reader_service_tests.cpp /Fo:work\build\reader_service_tests.obj /Fe:work\build\reader_service_tests.exe
if errorlevel 1 exit /b 1
work\build\reader_service_tests.exe
if errorlevel 1 exit /b 1
for %%t in (map_acquisition_tests destination_session_tests reader_core_tests background_discovery_tests build_compatibility_tests game_layout_detection_tests) do (
    cl /nologo /std:c++17 /EHsc /MT /W4 /I src tests\%%t.cpp /Fo:work\build\%%t.obj /Fe:work\build\%%t.exe
    if errorlevel 1 exit /b 1
    work\build\%%t.exe
    if errorlevel 1 exit /b 1
)
cl /nologo /std:c++17 /EHsc /MT /O2 /W4 /LD /I work\reference\reshade\include /I work\reference\reshade\deps\imgui src\addon.cpp /Fo:work\build\addon.obj /link /OUT:outputs\local-test-1.0.7\build\CrimsonNavigationLab.addon64 /IMPLIB:work\build\CrimsonNavigationLab.lib
if errorlevel 1 exit /b 1
cl /nologo /std:c++17 /EHsc /MT /W4 tests\host_probe.cpp /Fo:work\build\host_probe.obj /Fe:work\build\host_probe.exe /link /IMPLIB:work\build\host_probe.lib
if errorlevel 1 exit /b 1
work\build\host_probe.exe
exit /b %errorlevel%
