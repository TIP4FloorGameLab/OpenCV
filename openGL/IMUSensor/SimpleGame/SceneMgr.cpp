#include "stdafx.h"
#include "SceneMgr.h"
#include "Function.cpp"

Renderer *g_Renderer = NULL;


SceneMgr::SceneMgr()
{
	g_Renderer = new Renderer(WINDOW_WIDTH, WINDOW_HEIGHT);

	if (!g_Renderer->IsInitialized())
	{
		std::cout << "Renderer could not be initialized.. \n";
	}
	isEdge = false;
	XTime = 0.f;

}


SceneMgr::~SceneMgr()
{
	sp.ClosePort();
	delete g_Renderer;
}

void SceneMgr::SetupSerialPort(string filter_sensor_id, CString port_number) {
	sp.filter_sensor_id = filter_sensor_id;

	if (sp.OpenPort(L"//./" + port_number)) {
		redible = true;
		sp.ConfigurePort(CBR_115200, 8, FALSE, NOPARITY, ONESTOPBIT);
		sp.SetCommunicationTimeouts(0, 0, 0, 0, 0);

	}
	else {
		cout << "��Ʈ ���� �����ְų� ����" << endl;
		redible = false;
	}
}

void SceneMgr::SaveResetNode() {
	reset_buffer.euler_x = -sp.parsedata.euler_x;
	reset_buffer.euler_y = -sp.parsedata.euler_y;
	reset_buffer.euler_z = -sp.parsedata.euler_z;


	//reset_buffer.acc_x = -sp.parsedata.acc_x;
	//reset_buffer.acc_y = -sp.parsedata.acc_y;
	//reset_buffer.acc_z = -sp.parsedata.acc_z;

	reset_graph_pos = true;
}

void SceneMgr::ReadDataAll(vector<ParseNode>& input_buffer) {
	// �� �ϳ��� �����͸� ������ �ɴϴ�!~ (�� ��)
	string_buffer.clear();
	sp.ReadByteStr(string_buffer);
	string_container.push_back(string_buffer);
	sp.Parsing(string_buffer);

	// �����͸� �� �� �޾ƿ��� �Ľ��ؼ� �Ķ���ͷ� �޾ƿ� ���Ϳ� ����~!
	// �������� �޾ƿ� ID�� ó���� �������� ����ID�� ��ġ �� ��쿡�� ������ �Է�
	if (sp.parsedata.str_id == sp.filter_sensor_id) {
		ParseNode buffer;
		buffer.str_id = sp.parsedata.str_id;
		buffer.distance_x = sp.parsedata.distance_x;
		buffer.distance_y = sp.parsedata.distance_y;
		buffer.distance_z = sp.parsedata.distance_z;

		//buffer.euler_x = sp.parsedata.euler_x;
		//buffer.euler_y = sp.parsedata.euler_y;
		//buffer.euler_z = sp.parsedata.euler_z;
		//buffer.gyro_x = sp.parsedata.gyro_x;
		//buffer.gyro_y = sp.parsedata.gyro_y;
		//buffer.gyro_z = sp.parsedata.gyro_z;
		buffer.battery = sp.parsedata.battery;
		input_buffer.push_back(buffer);

		if (!isInit)
		{
			pre_AM[0] = pre_GM[0] = pre_HM[0] = pre_MM[0] = pre_LM[0] = pre_QM[0] = pre_IM[0] = sp.parsedata.distance_x;
			pre_AM[1] = pre_GM[1] = pre_HM[1] = pre_MM[1] = pre_LM[1] = pre_QM[1] = pre_IM[1] = sp.parsedata.distance_y;
			pre_AM[2] = pre_GM[2] = pre_HM[2] = pre_MM[2] = pre_LM[2] = pre_QM[2] = pre_IM[2] = sp.parsedata.distance_z;
			isInit = true;
		}
	}
	else {
		cout << "��ġ�ϴ� ������ ��� ���� �� ����" << endl;
	}
}

// ��հ� ����ϴ� �Լ�
void SceneMgr::DrawAvgText() {
	float diff_x = sp.parsedata.euler_x + reset_buffer.euler_x - pure_angle_x;
	float diff_y = sp.parsedata.euler_y + reset_buffer.euler_y - pure_angle_y;
	float diff_z = sp.parsedata.euler_z + reset_buffer.euler_z - pure_angle_z;
	char write_data[256] = { 0 };
	sprintf(write_data, "Refined - Angle \n  X : %.2f,  Y : %.2f, Z : %.2f \n", diff_x, diff_y, diff_z);
	g_Renderer->DrawTextA(100.f, -250.f, GLUT_BITMAP_TIMES_ROMAN_24
		, 1.f, 1.f, 1.f, write_data);
}

void SceneMgr::DrawErrText() {
	char write_data[256] = { 0 };
	sprintf(write_data, "Your port is not exist.");
	g_Renderer->DrawTextA(0.f, 0.f, GLUT_BITMAP_TIMES_ROMAN_24
		, 1.f, 0.f, 0.f, write_data);

}

void SceneMgr::DrawLayout() {
	// ���μ� 
	for (int i = 0; i < 100; ++i) {
		g_Renderer->DrawSolidRect(
			0.f, -500.f + 10 * i, 0.f,
			3.f,
			1.f, 1.f, 1.f,
			1.f, 0.9f);
	}

}
void SceneMgr::DrawObject()
{
	if (isEdge)
	{
		for (int n = 0; n < 3; ++n)
		{
			PullData(AM[n]);
			PullData(GM[n]);
			PullData(HM[n]);
			PullData(MM[n]);
			PullData(LM[n]);
			PullData(QM[n]);
			PullData(IM[n]);
		}
	}

	int size = 0;
	//Ű�Է����� 0~7������ ���ڸ� ������ NOW_MEAN�� ����ǰ�
	//�׿� ���� ���ʿ� �ش�Ǵ� ����� �׷����� �׷����ϴ�.
	switch (NOW_MEAN)
	{
	case Weighted:
		size = Weighted_Value->size();
		break;
	case Arithmetic:
		size = AM->size();
		break;
	case Geometric:
		size = GM->size();
		break;
	case Harmonic:
		size = HM->size();
		break;
	case Median:
		size = MM->size();
		break;
	case Log:
		size = LM->size();
		break;
	case Quadratic:
		size = QM->size();
		break;
	case Interquartile:
		size = IM->size();
		break;
	}


	for (int i = 0; i < size; ++i)
	{	
		//'R'��ư�� ������ ��
		if (reset_graph_pos) {
			cout << "�׷��� �ʱ�ȭ" << endl;
			
			a_reset_graph_x = -AM[0][i].second;
			a_reset_graph_y = -AM[1][i].second;
			a_reset_graph_z = -AM[2][i].second;

			g_reset_graph_x = -GM[0][i].second;
			g_reset_graph_y = -GM[1][i].second;
			g_reset_graph_z = -GM[2][i].second;

			h_reset_graph_x = -HM[0][i].second;
			h_reset_graph_y = -HM[1][i].second;
			h_reset_graph_z = -HM[2][i].second;

			m_reset_graph_x = -MM[0][i].second;
			m_reset_graph_y = -MM[1][i].second;
			m_reset_graph_z = -MM[2][i].second;

			l_reset_graph_x = -LM[0][i].second;
			l_reset_graph_y = -LM[1][i].second;
			l_reset_graph_z = -LM[2][i].second;

			q_reset_graph_x = -QM[0][i].second;
			q_reset_graph_y = -QM[1][i].second;
			q_reset_graph_z = -QM[2][i].second;

			i_reset_graph_x = -IM[0][i].second;
			i_reset_graph_y = -IM[1][i].second;
			i_reset_graph_z = -IM[2][i].second;

			reset_graph_pos = false;
		}
		switch (NOW_MEAN)
		{
		//0~7�� ����Ű�� ������ ��
		case Weighted:
			g_Renderer->DrawSolidRect(Weighted_Value[0][i].first - WINDOW_WIDTH / 2, Weighted_Value[0][i].second  + 150.f *  1.f, 0, 3, 1.f, 0.f, 1.f, 1.f, LEVEL_RECT);
			g_Renderer->DrawSolidRect(Weighted_Value[1][i].first - WINDOW_WIDTH / 2, Weighted_Value[1][i].second  + 150.f *  0.f, 0, 3, 1.f, 0.f, 1.f, 1.f, LEVEL_RECT);
			g_Renderer->DrawSolidRect(Weighted_Value[2][i].first - WINDOW_WIDTH / 2, Weighted_Value[2][i].second  + 150.f * -1.f, 0, 3, 1.f, 0.f, 1.f, 1.f, LEVEL_RECT);
			break;
		case Arithmetic:
			g_Renderer->DrawSolidRect(AM[0][i].first - WINDOW_WIDTH / 2, AM[0][i].second + a_reset_graph_x + 150.f *  1.f  , 0, 3, 1.f, 0.f, 0.f, 1.f, LEVEL_RECT);
			g_Renderer->DrawSolidRect(AM[1][i].first - WINDOW_WIDTH / 2, AM[1][i].second + a_reset_graph_y + 150.f *  0.f  , 0, 3, 1.f, 0.f, 0.f, 1.f, LEVEL_RECT);
			g_Renderer->DrawSolidRect(AM[2][i].first - WINDOW_WIDTH / 2, AM[2][i].second + a_reset_graph_z + 150.f * -1.f , 0, 3, 1.f, 0.f, 0.f, 1.f, LEVEL_RECT);
			break;
		case Geometric:
			g_Renderer->DrawSolidRect(GM[0][i].first - WINDOW_WIDTH / 2, GM[0][i].second + g_reset_graph_x + 150.f *  1.f, 0, 3, 0.f, 1.f, 0.f, 1.f, LEVEL_RECT);
			g_Renderer->DrawSolidRect(GM[1][i].first - WINDOW_WIDTH / 2, GM[1][i].second + g_reset_graph_y + 150.f *  0.f, 0, 3, 0.f, 1.f, 0.f, 1.f, LEVEL_RECT);
			g_Renderer->DrawSolidRect(GM[2][i].first - WINDOW_WIDTH / 2, GM[2][i].second + g_reset_graph_z + 150.f * -1.f, 0, 3, 0.f, 1.f, 0.f, 1.f, LEVEL_RECT);
			break;
		case Harmonic:
			g_Renderer->DrawSolidRect(HM[0][i].first - WINDOW_WIDTH / 2, HM[0][i].second + h_reset_graph_x + 150.f *  1.f, 0, 3, 0.f, 0.0, 1.0, 1.f, LEVEL_RECT);
			g_Renderer->DrawSolidRect(HM[1][i].first - WINDOW_WIDTH / 2, HM[1][i].second + h_reset_graph_y + 150.f *  0.f, 0, 3, 0.f, 0.0, 1.0, 1.f, LEVEL_RECT);
			g_Renderer->DrawSolidRect(HM[2][i].first - WINDOW_WIDTH / 2, HM[2][i].second + h_reset_graph_z + 150.f * -1.f, 0, 3, 0.f, 0.0, 1.0, 1.f, LEVEL_RECT);
			break;
		case Median:
			g_Renderer->DrawSolidRect(MM[0][i].first - WINDOW_WIDTH / 2, MM[0][i].second + m_reset_graph_x + 150.f *  1.f, 0, 3, 1.f, 1.f, 0.f, 1.f, LEVEL_RECT);
			g_Renderer->DrawSolidRect(MM[1][i].first - WINDOW_WIDTH / 2, MM[1][i].second + m_reset_graph_y + 150.f *  0.f, 0, 3, 1.f, 1.f, 0.f, 1.f, LEVEL_RECT);
			g_Renderer->DrawSolidRect(MM[2][i].first - WINDOW_WIDTH / 2, MM[2][i].second + m_reset_graph_z + 150.f * -1.f, 0, 3, 1.f, 1.f, 0.f, 1.f, LEVEL_RECT);
			break;
		case Log:
			g_Renderer->DrawSolidRect(LM[0][i].first - WINDOW_WIDTH / 2, LM[0][i].second + l_reset_graph_x + 150.f *  1.f, 0, 3, 0.f, 1.f, 1.f, 1.f, LEVEL_RECT);
			g_Renderer->DrawSolidRect(LM[1][i].first - WINDOW_WIDTH / 2, LM[1][i].second + l_reset_graph_y + 150.f *  0.f, 0, 3, 0.f, 1.f, 1.f, 1.f, LEVEL_RECT);
			g_Renderer->DrawSolidRect(LM[2][i].first - WINDOW_WIDTH / 2, LM[2][i].second + l_reset_graph_z + 150.f * -1.f, 0, 3, 0.f, 1.f, 1.f, 1.f, LEVEL_RECT);
			break;
		case Quadratic:
			g_Renderer->DrawSolidRect(QM[0][i].first - WINDOW_WIDTH / 2, QM[0][i].second + q_reset_graph_x + 150.f *  1.f, 0, 3, 1.f, 1.f, 1.f, 1.f, LEVEL_RECT);
			g_Renderer->DrawSolidRect(QM[1][i].first - WINDOW_WIDTH / 2, QM[1][i].second + q_reset_graph_y + 150.f *  0.f, 0, 3, 1.f, 1.f, 1.f, 1.f, LEVEL_RECT);
			g_Renderer->DrawSolidRect(QM[2][i].first - WINDOW_WIDTH / 2, QM[2][i].second + q_reset_graph_z + 150.f * -1.f, 0, 3, 1.f, 1.f, 1.f, 1.f, LEVEL_RECT);
			break;
		case Interquartile:
			g_Renderer->DrawSolidRect(IM[0][i].first - WINDOW_WIDTH / 2, IM[0][i].second + i_reset_graph_x + 150.f *  1.f, 0, 3, 0.5, 0.5, 0.5, 1.f, LEVEL_RECT);
			g_Renderer->DrawSolidRect(IM[1][i].first - WINDOW_WIDTH / 2, IM[1][i].second + i_reset_graph_y + 150.f *  0.f, 0, 3, 0.5, 0.5, 0.5, 1.f, LEVEL_RECT);
			g_Renderer->DrawSolidRect(IM[2][i].first - WINDOW_WIDTH / 2, IM[2][i].second + i_reset_graph_z + 150.f * -1.f, 0, 3, 0.5, 0.5, 0.5, 1.f, LEVEL_RECT);
			break;
		default:
			break;
		}
	}
}

void SceneMgr::DrawCubeInRaw_CASE_nonsenser(float xPos, float yPos) {
	//ROOT ť��
	g_Renderer->DrawSolidCube(xPos, yPos, 0.f,
		(ROOT.x)* PI / 180.f,
		(ROOT.y) * PI / 180.f,
		(ROOT.z) * PI / 180.f, 125.f);

	//JOINT0 ť��
	g_Renderer->DrawSolidCube(xPos, yPos + 150.f, 0.f,
		(JOINT0.x)* PI / 180.f,
		(JOINT0.y) * PI / 180.f,
		(JOINT0.z) * PI / 180.f, 125.f);
}

void SceneMgr::DrawCubeInRaw() {
	switch (NOW_MEAN)
	{
	case Arithmetic:
		g_Renderer->DrawSolidCube(-375.f, 0.f, 0.f,
			((pre_AM[0]) + 180.f)* PI / 180.f,   
			((pre_AM[1]) + 180.f) * PI / 180.f,
			((pre_AM[2]) + 180.f) * PI / 180.f, 250.f);
		break;
	case Geometric:
		g_Renderer->DrawSolidCube(-375.f, 0.f, 0.f,
			((pre_GM[0]) + 180.f)* PI / 180.f,
			((pre_GM[1]) + 180.f) * PI / 180.f,
			((pre_GM[2]) + 180.f) * PI / 180.f, 250.f);
		break;
	case Harmonic:
		g_Renderer->DrawSolidCube(-375.f, 0.f, 0.f,
			((pre_HM[0]) + 180.f)* PI / 180.f,
			((pre_HM[1]) + 180.f) * PI / 180.f,
			((pre_HM[2]) + 180.f) * PI / 180.f, 250.f);
		break;
	case Median:
		g_Renderer->DrawSolidCube(-375.f, 0.f, 0.f,
			((pre_MM[0]) + 180.f)* PI / 180.f,
			((pre_MM[1]) + 180.f) * PI / 180.f,
			((pre_MM[2]) + 180.f) * PI / 180.f, 250.f);
		break;
	case Log:
		g_Renderer->DrawSolidCube(-375.f, 0.f, 0.f,
			((pre_LM[0]) + 180.f)* PI / 180.f,
			((pre_LM[1]) + 180.f) * PI / 180.f,
			((pre_LM[2]) + 180.f) * PI / 180.f, 250.f);
		break;
	case Quadratic:
		g_Renderer->DrawSolidCube(-375.f, 0.f, 0.f,
			((pre_QM[0]) + 180.f)* PI / 180.f,
			((pre_QM[1]) + 180.f) * PI / 180.f,
			((pre_QM[2]) + 180.f) * PI / 180.f, 250.f);
		break;
	case Interquartile:
		g_Renderer->DrawSolidCube(-375.f, 0.f, 0.f,
			((pre_IM[0]) + 180.f)* PI / 180.f,
			((pre_IM[1]) + 180.f) * PI / 180.f,
			((pre_IM[2]) + 180.f) * PI / 180.f, 250.f);
		break;
	default:
		break;
	}
}

void SceneMgr::DrawCubeInWeight() {
	float draw_weight_x = 0.f;
	float draw_weight_y = 0.f;
	float draw_weight_z = 0.f;

	draw_weight_x =
		pre_AM[0] * WEIGHT_A +
		pre_GM[0] * WEIGHT_G +
		pre_HM[0] * WEIGHT_H +
		pre_IM[0] * WEIGHT_I +
		pre_LM[0] * WEIGHT_L +
		pre_MM[0] * WEIGHT_M +
		pre_QM[0] * WEIGHT_Q;

	draw_weight_y =
		pre_AM[1] * WEIGHT_A +
		pre_GM[1] * WEIGHT_G +
		pre_HM[1] * WEIGHT_H +
		pre_IM[1] * WEIGHT_I +
		pre_LM[1] * WEIGHT_L +
		pre_MM[1] * WEIGHT_M +
		pre_QM[1] * WEIGHT_Q;

	draw_weight_z =
		pre_AM[2] * WEIGHT_A +
		pre_GM[2] * WEIGHT_G +
		pre_HM[2] * WEIGHT_H +
		pre_IM[2] * WEIGHT_I +
		pre_LM[2] * WEIGHT_L +
		pre_MM[2] * WEIGHT_M +
		pre_QM[2] * WEIGHT_Q;

	Weighted_Value[0].push_back(make_pair(XTime, draw_weight_x));
	Weighted_Value[1].push_back(make_pair(XTime, draw_weight_y));
	Weighted_Value[2].push_back(make_pair(XTime, draw_weight_z));

	draw_weight_x = ((draw_weight_x) + 180.f)*PI / 180.f;
	draw_weight_y = ((draw_weight_y) + 180.f)*PI / 180.f;
	draw_weight_z = ((draw_weight_z) + 180.f)*PI / 180.f;
	

	g_Renderer->DrawSolidCube(-375.f, 0.f, 0.f, 
		draw_weight_x, draw_weight_y, draw_weight_z, 250.f);
}

void SceneMgr::DrawEulerData(float elapsed_time) {
	refined_angle_x = ((sp.parsedata.euler_x + reset_buffer.euler_x) + 180.f)*PI / 180.f;
	refined_angle_y = ((sp.parsedata.euler_y + reset_buffer.euler_y) + 180.f)*PI / 180.f;
	refined_angle_z = ((sp.parsedata.euler_z + reset_buffer.euler_z) + 180.f)*PI / 180.f;

	if (sp.filter_sensor_id == sp.parsedata.str_id)
		g_Renderer->DrawSolidCube(375.f, 0.f, 0.f,
			refined_angle_x,
			refined_angle_y,
			refined_angle_z, 250.f);
}

void SceneMgr::DrawGyroData(float elapsed_time) {
	//pure_angle_x = pure_angle_x + (sp.parsedata.gyro_x + reset_buffer.gyro_x) * elapsed_time;
	//pure_angle_y = pure_angle_y + (sp.parsedata.gyro_y + reset_buffer.gyro_y) * elapsed_time;
	//pure_angle_z = pure_angle_z + (sp.parsedata.gyro_z + reset_buffer.gyro_z) * elapsed_time;


	// ���ӵ��� ������ degree / sec �̴�. 
	// �츮�� �׸��� DrawSolidCube�� ������ �̿��ؼ� �׸���.
	// �� ���� ũ�⸸ ���;��Ѵ�. 
	// �׷��Ƿ� (degree / sec) * sec �� ����� ���� �׸� �� �ִ�. 

	g_Renderer->DrawSolidCube(-375.f, 0.f, 0.f,
		((pure_angle_x - reset_buffer.euler_x) + 180.f) * PI / 180.f,
		((pure_angle_y - reset_buffer.euler_y) + 180.f) * PI / 180.f,
		((pure_angle_z - reset_buffer.euler_z) + 180.f) * PI / 180.f, 250.f);
}


void SceneMgr::CopyData(vector<ParseNode>& input_buffer)
{
	// input_buffer�� render_buffer�� �������� ���ش�.
	render_buffer.clear();
	copy(input_buffer.begin(), input_buffer.end(), back_inserter(render_buffer));

	//render_buffer�� �ɰ��� �� ���Ϳ� �����Ѵ�.
	for (auto d : render_buffer)
	{
		//�� ���� ���� �ڿ� 
		//gyro_x.push_back(d.gyro_x);
		//gyro_y.push_back(d.gyro_y);
		//gyro_z.push_back(d.gyro_z);
	}
}


void SceneMgr::UpdateAllObejct(float elapsed_time)
{   // �������� render_buffer���� ����� ���ش�.
	MeanData(gyro_x, OP_GYRO_X, elapsed_time);
	MeanData(gyro_y, OP_GYRO_Y, elapsed_time);
	MeanData(gyro_z, OP_GYRO_Z, elapsed_time);
}

void SceneMgr::MeanData(vector<float>& v, int parse_option, float elapsed_time)
{
	int n = parse_option;
	//parse_option�� ���� gyro_x, y,z�� �ش� �迭�� ���� 
	//'v�� ����ִ� gyro���� ����'�� ����� ����Ͽ� �����մϴ�.
	point.Apoint[n] = Arithmetic_Mean(v);
	point.Gpoint[n] = Geometric_Mean(v);
	point.Hpoint[n] = Harmonic_Mean(v);
	point.Mpoint[n] = Median_Mean(v);
	point.Lpoint[n] = Log_Mean(v);
	point.Qpoint[n] = Quadratic_Mean(v);
	point.Ipoint[n] = Interquartile_Mean(v);

	//�� ���� ������ ������ ���鿡 �ð�(����)�� ���ϸ� ���ο� ���� �߰��˴ϴ�.
	//���⼭ pre_#M�� ������ ���� ���� ������ ���� �ֽ��ϴ�.
	//pre_#M�� ���ο� ���� ������ ���� �� �� ������ �����ϴ�.
	pre_AM[n] += point.Apoint[n] * elapsed_time;
	pre_GM[n] += point.Gpoint[n] * elapsed_time;
	pre_HM[n] += point.Hpoint[n] * elapsed_time;
	pre_MM[n] += point.Mpoint[n] * elapsed_time;
	pre_LM[n] += point.Lpoint[n] * elapsed_time;
	pre_QM[n] += point.Qpoint[n] * elapsed_time;
	pre_IM[n] += point.Ipoint[n] * elapsed_time;

	//#M���Ϳ� ��� ���� ���� ������ 
	//XTime(;���⼭ X��ǥ�� �ش�) �� push_back���ݴϴ�.
	AM[n].push_back(make_pair(XTime, pre_AM[n]));
	GM[n].push_back(make_pair(XTime, pre_GM[n]));
	HM[n].push_back(make_pair(XTime, pre_HM[n]));
	MM[n].push_back(make_pair(XTime, pre_MM[n]));
	LM[n].push_back(make_pair(XTime, pre_LM[n]));
	QM[n].push_back(make_pair(XTime, pre_QM[n]));
	IM[n].push_back(make_pair(XTime, pre_IM[n]));

	//v���͸� ����ݴϴ�.
	v.clear();
}

void SceneMgr::PullData(vector<INFO>& M)
{	
	int count = 0;
	auto i = M.begin();
	auto iter = M.erase(i);

	for (auto i = iter; i < M.end();++i)
	{
		(*i).first -= deltaTime;
		++count;
	}

}

void SceneMgr::DrawCube() {
	// Matrix Update
	// �̵� * ȸ�� * ũ�� * Vector ������ 
	//int mov_x = 0;
	//int mov_y = 0;
	//int mov_z = 0;
	//int scale_x = 1;
	//int scale_y = 1;
	//int scale_z = 1;
	//int rot_x = 0;
	//int rot_y = 0;
	//int rot_z = 0;

	//glm::mat4 rotate_matrix_x{
	//	glm::vec4{ 1,0,0,0 },
	//	glm::vec4{ 0,cos(rot_x),-sin(rot_x),0 },
	//	glm::vec4{ 0,sin(rot_x),cos(rot_x),0 },
	//	glm::vec4{ 0,0,0,1 } };

	//glm::mat4 rotate_matrix_y{
	//	glm::vec4{ cos(rot_y),0,sin(rot_y),0 },
	//	glm::vec4{ 0,1,0,0 },
	//	glm::vec4{ -sin(rot_y),0,cos(rot_y),0 },
	//	glm::vec4{ 0,0,0,1 } };

	//glm::mat4 rotate_matrix_z{
	//	glm::vec4{ cos(rot_z),-sin(rot_z),0,0 },
	//	glm::vec4{ sin(rot_z),cos(rot_z),0,0 },
	//	glm::vec4{ 0,0,1,0 },
	//	glm::vec4{ 0,0,0,1 } };

	//rotate_matrix = rotate_matrix_x * rotate_matrix_y * rotate_matrix_z;

	//moving_matrix = {
	//	glm::vec4{ 1,0,0,mov_x },
	//	glm::vec4{ 0,1,0,mov_y },
	//	glm::vec4{ 0,0,1,mov_z },
	//	glm::vec4{ 0,0,0,1 } };

	//scaling_matrix = {
	//	glm::vec4{ scale_x,0,0,0 },
	//	glm::vec4{ 0,scale_y,0,0 },
	//	glm::vec4{ 0,0,scale_z,0 },
	//	glm::vec4{ 0,0,0,1 }
	//};


	//transforming_matrix = moving_matrix * rotate_matrix * scaling_matrix;

	g_Renderer->DrawSolidCube(0.f, 0.f, 0.f,
		0.f, 0.f, 0.f, SIZE_OF_CUBE*0.1);
	// ������ ť�� 
	/*g_Renderer->DrawSolidCube(
		BETWEEN_LENGTH * sin(sp.parsedata.euler_y / 180.f * 3.141592),
		BETWEEN_LENGTH * cos(sp.parsedata.euler_y / 180.f * 3.141592),
		0.f,
		0.f,
		0.f,
		-sp.parsedata.euler_y / 180.f * 3.141592,
		SIZE_OF_CUBE);*/

	//g_Renderer->DrawSolidCube(
	//	sp.parsedata.distance_x  ,
	//	sp.parsedata.distance_y ,
	//	sp.parsedata.distance_z ,
	//	-sp.parsedata.euler_x / 180.f * PI,
	//	-sp.parsedata.euler_y / 180.f * PI,
	//	-sp.parsedata.euler_z / 180.f * PI,
	//	SIZE_OF_CUBE);


	g_Renderer->DrawSolidCube(
		sp.parsedata.distance_x*10,
		sp.parsedata.distance_y * 10,
		sp.parsedata.distance_z * 10,
		0,
		0,
		0,
		SIZE_OF_CUBE);




}