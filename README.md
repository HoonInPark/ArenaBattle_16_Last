# ArenaBattle_16_Last
&lt;&lt;이득우의 언리얼 C++ 게임 개발의 정석>>의 Unreal 5.2.1 버전입니다. 자유롭게 다운 받아서 공부하는 데에 쓰세요~
커밋 메시지의 숫자는 교과서 상의 페이지를 의미합니다

교과서는 Ureal 4를 기준으로 만들어졌지만, 이를 Unreal 5로 작성하게 되면 다음 포인트에서 그 구현 방식이 크게 차이납니다.
1. 키매핑을 해주는 방식. 
2. Anim Bluepirnt를 C++ Pawn 클래스와 연결하는 방식 - TryGetPawnOwner()를 사용하는 방식과 UInterface를 이용하는 방식 두가지가 있습니다.
3. ->Relative가 아니라 Get/SetRelative~
4. Animation Retargeting
5. Animation Montage Section을 조작하는 방법에 있어서 Editor UI의 변화.
 
 위 내용은 다음 유튜브 강의로 다룹니다.
 이득우_1 : https://youtu.be/uf8PAxifIlI
 이득우_2 : 