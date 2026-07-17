#pragma once

#include <memory>
#include <string>
#include <vector>

// LoginScene camera offset defaults.
// WD_73NEW_LOGIN_SCENE (= 73) usa iMapWorld = 74, portanto:
//   - Pasta de dados: Data/World74, Data/Object74
//   - Script de camera: Data/World74/CWScript74.cws
// O código original (pre-merge) NÃO tinha offsets — os waypoints do
// CWScript74.cws já incorporam as posições corretas para a fórmula original.
namespace LoginSceneCameraDefaults
{
    constexpr float OFFSET_X = 0.0f;
    constexpr float OFFSET_Y = 0.0f;
    constexpr float OFFSET_Z = 1000.0f;
    // ANGLE_PITCH = 0 → ângulo final -112.5° (idêntico ao código original)
    constexpr float ANGLE_PITCH = 0.0f;
    constexpr float ANGLE_YAW   = 25.0f;

    // Default render distances for LoginScene (tour camera).
    // DevEditor exposes these as sliders; release builds use these as fixed values.
    constexpr float RENDER_TERRAIN_DIST = 3995.0f;
    constexpr float RENDER_OBJECT_DIST  = 5903.0f;
}

// Runtime-adjustable offsets (initially set to the defaults above)
extern float g_LoginSceneOffsetX;
extern float g_LoginSceneOffsetY;
extern float g_LoginSceneOffsetZ;
extern float g_LoginSceneAnglePitch;
extern float g_LoginSceneAngleYaw;

class CCameraMove
{
#pragma pack(push, 1)
    struct WAYPOINT {
        int iIndex;

        float fCameraX;
        float fCameraY;
        float fCameraZ;

        int iDelay;
        float fCameraMoveAccel;
        float fCameraDistanceLevel;
    };
#pragma pack(pop)

    using t_WayPointList = std::vector<std::unique_ptr<WAYPOINT>>;
    t_WayPointList m_listWayPoint;

    float m_CameraStartPos[3];
    float m_fCameraStartDistanceLevel;
    double m_iDelayCount;

    DWORD m_dwCameraWalkState;
    float m_CurrentCameraPos[3];
    float m_fCurrentDistanceLevel;

    DWORD	m_dwCurrentIndex;
    int		m_iSelectedTile;

    void Init();
public:
    enum {
        CAMERAWALK_STATE_READY = 0,
        CAMERAWALK_STATE_MOVE,
        CAMERAWALK_STATE_DONE,
    };
    ~CCameraMove();

    bool LoadCameraWalkScript(const std::wstring& filename);
    void UnLoadCameraWalkScript();
    bool SaveCameraWalkScript(const std::wstring& filename);

    void AddWayPoint(int iGridX, int iGridY, float fCameraMoveAccel, float fCameraDistanceLevel, int iDelay);
    void RemoveWayPoint(int iGridX, int iGridY);

    void SetCameraMoveAccel(int iTileIndex, float fCameraMoveAccel);
    void SetCameraDistanceLevel(int iTileIndex, float fCameraDistanceLevel);
    void SetDelay(int iTileIndex, int iDelay);
    float GetCameraMoveAccel(int iTileIndex);
    float GetCameraDistanceLevel(int iTileIndex);
    int GetDelay(int iTileIndex);

    bool IsCameraMove() const;
    void UpdateWayPoint();
    void GetCurrentCameraPos(float CameraPos[3]);
    float GetCurrentCameraDistanceLevel() const;

    void PlayCameraWalk(float StartPos[3], float fStartDistanceLevel);
    void StopCameraWalk(bool bDone);
    void UpdateCameraStartPos(float StartPos[3]);

    void SetCameraWalkState(DWORD dwCameraWalkState);
    DWORD GetCameraWalkState() const;

    void RenderWayPoint();
    void SetSelectedTile(int iTileIndex);
    DWORD GetSelectedTile() const;

    static CCameraMove* GetInstancePtr();

public:
    CCameraMove();

private:
    WAYPOINT* GetWayPointByIndex(std::size_t index);
    const WAYPOINT* GetWayPointByIndex(std::size_t index) const;
    WAYPOINT* FindWayPointByTile(int tileIndex);
    const WAYPOINT* FindWayPointByTile(int tileIndex) const;

    BOOL m_bTourMode;
    BOOL m_bTourPause;
    float m_fForceSpeed;
    float m_fTourCameraAngle;
    float m_fTargetTourCameraAngle;
    float m_vTourCameraPos[3];
    float m_fCameraAngle;
    float m_fFrustumAngle;
public:
    BOOL SetTourMode(BOOL bFlag, BOOL bRandomStart = FALSE, int _index = 0);
    BOOL IsTourMode() { return m_bTourMode; }
    BOOL IsTourPaused() { return m_bTourPause; }
    void PauseTour(BOOL bFlag);
    void ForwardTour(float fSpeed);
    void BackwardTour(float fSpeed);
    void UpdateTourWayPoint();
    void SetAngleFrustum(float _Value);
    const float GetAngleFrustum() const { return m_fCameraAngle; };
    void SetFrustumAngle(float _Value);
    float GetFrustumAngle();

    float GetCameraAngle() { return m_fTourCameraAngle; }
};
