#pragma once
#include "Renderer.h"
#include "SerialPort.h"
//------------------------------------------

class SceneMgr
{
public:
	//센서 값이 있을 때
	vector<float> gyro_x;
	vector<float> gyro_y;
	vector<float> gyro_z;

	vector<float> acc_x;
	vector<float> acc_y;
	vector<float> acc_z;


	//-----시간 값과 각 평균값을 저장하는 컨테이너----
	vector<INFO> AM[3];
	vector<INFO> GM[3];
	vector<INFO> HM[3];
	vector<INFO> MM[3];
	vector<INFO> LM[3];
	vector<INFO> QM[3];
	vector<INFO> IM[3];
	vector<INFO> Weighted_Value[3];  //가중치가 반영된 그래프를 

	//-----가장 최신 업데이트 점 평균 별로
	float pre_AM[3];
	float pre_GM[3];
	float pre_HM[3];
	float pre_MM[3];
	float pre_LM[3];
	float pre_QM[3];
	float pre_IM[3];
	float pre_WV[3];

	boolean isInit = false;

	int NOW_MEAN = 0;	
	
	//센서 값이 없을 때
	Vec	ROOT = { 0,0,0 };
	Vec JOINT0 = { 0,0,0 };
	
	void SetRootPos(Vec vec) { ROOT.x = vec.x; ROOT.y = vec.y; ROOT.z = vec.z;}

public:
	boolean isEdge;
	float XTime;

public:
	SerialPort sp;
	string string_buffer;
	vector<string> string_container;
	vector<ParseNode> parsing_container;
	vector<ParseNode> render_buffer;
	ParseNode reset_buffer;	// buffer의 가장 마지막 data를 저장
	Point point;							// 각 평균의 좌표값
	bool redible = false;			// 포트가 잘 연결되어있는지
	bool mode = false;				// mode = false -> Gyro
								// mode = true -> Acc

	// false = raw한 gyro값 기준으로 그리기
	// true = 가중치 적용한 값 기준으로 그리기
	bool draw_mode_selector = false;	


	float refined_angle_x = 0.f;
	float refined_angle_y = 0.f;
	float refined_angle_z = 0.f;

	float pure_angle_x = -180.f;
	float pure_angle_y = -180.f;
	float pure_angle_z = -180.f;
	
	// 활성화 되면 현 그래프를 전부 정위치 시킨다.
	bool reset_graph_pos = false;
	float a_reset_graph_x = 0.f;
	float a_reset_graph_y = 0.f;
	float a_reset_graph_z = 0.f;

	float g_reset_graph_x = 0.f;
	float g_reset_graph_y = 0.f;
	float g_reset_graph_z = 0.f;

	float h_reset_graph_x = 0.f;
	float h_reset_graph_y = 0.f;
	float h_reset_graph_z = 0.f;

	float m_reset_graph_x = 0.f;
	float m_reset_graph_y = 0.f;
	float m_reset_graph_z = 0.f;

	float l_reset_graph_x = 0.f;
	float l_reset_graph_y = 0.f;
	float l_reset_graph_z = 0.f;

	float q_reset_graph_x = 0.f;
	float q_reset_graph_y = 0.f;
	float q_reset_graph_z = 0.f;

	float i_reset_graph_x = 0.f;
	float i_reset_graph_y = 0.f;
	float i_reset_graph_z = 0.f;
public:
	SceneMgr();
	~SceneMgr();

	void UpdateAllObejct(float elapsed_time);

	void DrawObject();
	void DrawEulerData(float elapsed_time);
	void DrawGyroData(float elapsed_time);

	void CopyData(vector<ParseNode>& input_buffer);
	void MeanData(vector<float>& v, int parse_option, float elapsed_time);

	void SetupSerialPort(string filter_sensor_id, CString port_number);
	void ReadDataAll(vector<ParseNode>& input_buffer);

	void PullData(vector<INFO>& v);

	void SaveResetNode();		// buffer의 마지막 data를 받아와 저장

	void DrawLayout();
	void DrawAvgText();
	void DrawErrText();

	void DrawCubeInWeight();	// 가중치 이용해서 그리기
	void DrawCubeInRaw();

	void DrawCubeInRaw_CASE_nonsenser(float xPos, float yPos); //센서 값을 받지 않을 시에 초기값(0)으로 큐브를 그리는 함수



	// 2018/5 Cube회전 관련
	void DrawCube();


private:
	// 최종 행렬
	glm::mat4 transforming_matrix;

	// 이동 행렬
	glm::mat4 moving_matrix;
	// 회전 행렬
	glm::mat4 rotate_matrix;
	// 크기 행렬
	glm::mat4 scaling_matrix;
};

