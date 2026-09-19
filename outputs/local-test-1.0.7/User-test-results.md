# 1.0.7 patch 2.03 gameplay result

Status: **passed** for 1.0.7-test2.

Test on Steam build 25381195 / executable 1.0.0.2944 with an active navigation destination:

- 1.0.7-test1 normal startup and no reader internal error: **passed**. ReShade remained responsive, automatic discovery recovered, and the log advanced to a calculated road route.
- 1.0.7-test1 visible minimap route: **passed**, confirmed by the user.
- 1.0.7-test1 visible full-screen map route: **failed**, confirmed by the user.
- 1.0.7-test2 read-only live full-map projection: **passed**, 10/10 raw and 10/10 1920x1080 display samples against the running game.
- 1.0.7-test2 installed binary and deployment integrity: **passed**. Candidate/installed SHA256 is `BC5FB52B86966358C950E7BB8DF2B8DF017E7FF80C2189EADFB7D228C3AE35D8`; all 979 other `bin64` files are unchanged by path, size, and modification time.
- 1.0.7-test2 visible minimap route: **passed**, user reported `ok`.
- 1.0.7-test2 visible full-screen map route: **passed**, user reported `good`.
- 1.0.7-test2 full-map pan and zoom: **passed**, user reported `good`.
- 1.0.7-test2 ReShade health/no error: **passed**, user reported `good`.

The test1 failure was traced to a separate patch change: the full-map reference canvas and cursor/world transform moved by 16 bytes. Test2 discovers that paired block structurally within a bounded window and rejects ambiguous or changing memory. The saved runtime log corroborates 1.0.7-test2 loading, unique discovery of both maps, active route calculation, and repeated full-map route submissions. Scope is the requested startup, both-map, full-map pan/zoom, and ReShade-health check on the current setup; it does not establish universal resolution or Proton compatibility.

[Runtime log](startup-game-log-test2.txt), [prepared release](../release-1.0.7/RELEASE-INDEX.md).
