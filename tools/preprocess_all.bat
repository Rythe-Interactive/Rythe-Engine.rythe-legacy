cd ".\Rythe-Preprocessor\"
RytheTributary -v=2 -moduleroot=..\..\legion\engine\ -include=..\..\deps\include -module="core" -ex=deps -ex=glm -ex=autogen
RytheTributary -v=2 -moduleroot=..\..\legion\engine\ -include=..\..\deps\include -module="application" -ex=deps -ex=glm -ex=autogen
RytheTributary -v=2 -moduleroot=..\..\legion\engine\ -include=..\..\deps\include -module="rendering" -ex=deps -ex=imgui -ex=glm -ex=autogen
RytheTributary -v=2 -moduleroot=..\..\legion\engine\ -include=..\..\deps\include -module="physics" -ex=deps -ex=glm -ex=autogen
RytheTributary -v=2 -moduleroot=..\..\legion\engine\ -include=..\..\deps\include -module="audio" -ex=deps -ex=glm -ex=autogen
RytheTributary -v=2 -moduleroot=..\..\applications\ -include=..\..\deps\include -include=..\..\legion\engine -module="sandbox" -ex=deps -ex=glm -ex=autogen
pause