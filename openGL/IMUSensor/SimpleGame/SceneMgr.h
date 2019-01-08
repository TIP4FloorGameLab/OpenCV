#pragma once
#include "Renderer.h"
#include "SerialPort.h"
//------------------------------------------

class SceneMgr
{
public:
	//���� ���� ���� ��
	vector<float> gyro_x;
	vector<float> gyro_y;
	vector<float> gyro_z;

	vector<float> acc_x;
	vector<float> acc_y;
	vector<float> acc_z;


	//-----�ð� ���� �� ��հ��� �����ϴ� �����̳�----
	vector<INFO> AM[3];
	vector<INFO> GM[3];
	vector<INFO> HM[3];
	vector<INFO> MM[3];
	vector<INFO> LM[3];
	vector<INFO> QM[3];
	vector<INFO> IM[3];
	vector<INFO> Weighted_Value[3];  //����ġ�� �ݿ��� �׷����� 

	//-----���� �ֽ� ������Ʈ �� ��� ����
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
	
	//���� ���� ���� ��
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
	ParseNode reset_buffer;	// buffer�� ���� ������ data�� ����
	Point point;							// �� ����� ��ǥ��
	bool redible = false;			// ��Ʈ�� �� ����Ǿ��ִ���
	bool mode = false;				// mode = false -> Gyro
								// mode = true -> Acc

	// false = raw�� gyro�� �������� �׸���
	// true = ����ġ ������ �� �������� �׸���
	bool draw_mode_selector = false;	


	float refined_angle_x = 0.f;
	float refined_angle_y = 0.f;
	float refined_angle_z = 0.f;

	float pure_angle_x = -180.f;
	float pure_angle_y = -180.f;
	float pure_angle_z = -180.f;
	
	// Ȱ��ȭ �Ǹ� �� �׷����� ���� ����ġ ��Ų��.
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

	void SaveResetNode();		// buffer�� ������ data�� �޾ƿ� ����

	void DrawLayout();
	void DrawAvgText();
	void DrawErrText();

	void DrawCubeInWeight();	// ����ġ �̿��ؼ� �׸���
	void DrawCubeInRaw();

	void DrawCubeInRaw_CASE_nonsenser(float xPos, float yPos); //���� ���� ���� ���� �ÿ� �ʱⰪ(0)���� ť�긦 �׸��� �Լ�



	// 2018/5 Cubeȸ�� ����
	void DrawCube();


private:
	// ���� ���
	glm::mat4 transforming_matrix;

	// �̵� ���
	glm::mat4 moving_matrix;
	// ȸ�� ���
	glm::mat4 rotate_matrix;
	// ũ�� ���
	glm::mat4 scaling_matrix;
};

