#include "pch.h"
#include "Background.h"


CBackground::CBackground()
    : m_initialized(false)
    , m_fbo(0)
    , m_texture(0)
    , m_hDcCompatible(NULL)
    , m_hBitmap(NULL)
{
    //
    // ������� ��-���������
    //
    m_width = 800;
    m_height = 600;
}

CBackground::~CBackground()
{
    if (m_initialized)
    {
        glDeleteFramebuffers(1, &m_fbo);
        glDeleteTextures(1, &m_texture);
        free(m_textureData);
    }

    if (m_hDcCompatible)
    {
        DeleteDC(m_hDcCompatible);
    }
}

bool
CBackground::setup()
{

    m_textureData = (unsigned int *)malloc(m_width * m_height * 4);

    if (!m_textureData)
        return false;

    memset(m_textureData, 0, m_width * m_height * 4);

    HWND  hwndDesktop = GetDesktopWindow();

    HDC hDc = GetDC(hwndDesktop);

    if (!hDc)
    {
        return false;
    }

    m_hDcCompatible = CreateCompatibleDC(hDc);

    m_hBitmap = CreateCompatibleBitmap(hDc, m_width, m_height);

    SelectObject(m_hDcCompatible, m_hBitmap);

    SetBkColor(m_hDcCompatible, RGB(0, 0, 0));
    SetTextColor(m_hDcCompatible, RGB(255, 255, 0));

    ReleaseDC(hwndDesktop, hDc);

    // 1. ������ ��������
    glGenTextures(1, &m_texture);
    glBindTexture(GL_TEXTURE_2D, m_texture);

    // 2. ��������� ������ � ��������. ����� ������ � ����������!
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, m_width, m_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, m_textureData);

    // 3. ����������� ���������� (�����������!)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    // 4. ����������� �������� � framebuffer
    glGenFramebuffers(1, &m_fbo);
    glBindFramebuffer(GL_FRAMEBUFFER, m_fbo);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_texture, 0);

    // �������� ������� �����������
    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
        
    }

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glBindTexture(GL_TEXTURE_2D, 0);

    m_initialized = true;
    return true;
}

bool
CBackground::prepareFBOData()
{

    //
    // ������ ��������� ����� �������������� FBO
    //
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, m_fbo);       // ���� � ������������ FBO

    //--------------------
    // ��������� 
    //--------------------

    // 
    //  ��� ��������� ����� � �������������� �����
    //
    //glClearColor(0.0f, 0.5f, 0.0f, 1.0f);
    //glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    /*glBegin(GL_LINES);

    glVertex3f(0.0f, 0.0f, 0.0f);    // ������
    glVertex3f(50.0f, 0.0f, 0.0f);  // �����

    glEnd();*/


    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);  // �����������

    return true;
}

void
CBackground::Resize(unsigned int sX, unsigned int sY)
{

    if (sX == 0 || sY == 0)
        return;

    unsigned int* textureDataNew = (unsigned int* )malloc(sX * sY * 4);
    unsigned int* textureDataOld = NULL;

    if (!textureDataNew)
        return;

    memset(textureDataNew, 0, sX * sY * 4);


    HWND  hwndDesktop = GetDesktopWindow();

    HDC hDc = GetDC(hwndDesktop);

    if (!hDc) {

        return;
    }

    HBITMAP  hBitmap = CreateCompatibleBitmap(hDc, sX, sY);

    HBITMAP  hOldBitmap = (HBITMAP)SelectObject(m_hDcCompatible, hBitmap);

    textureDataOld = m_textureData;

    m_hBitmap = hBitmap;
    m_textureData = textureDataNew;


    glBindTexture(GL_TEXTURE_2D, m_texture);

    // ��������� ����� ����������� ��������
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, sX, sY, 0, GL_RGBA, GL_UNSIGNED_BYTE, m_textureData);

    glBindTexture(GL_TEXTURE_2D, 0);


    m_width = sX;
    m_height = sY;

    //
    // ����� ���������� ����� ������� �������
    //
    DeleteObject(hOldBitmap);
    ReleaseDC(hwndDesktop, hDc);

    if (textureDataOld)
        free(textureDataOld);
}

void
CBackground::PreDraw()
{

    //
    // ������ ��� ���� �� �������� !
    //
    if (m_height > 200)
    {
        for (int i = 0; i < m_width; i++)
        {
            *(m_textureData + (m_width * 200) + i) = 0xFFFFFFFF;
        }
    }

    glBindTexture(GL_TEXTURE_2D, m_texture);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, m_width, m_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, m_textureData);

    glBindTexture(GL_TEXTURE_2D, 0);

}

void
CBackground::Draw(CRect &drawRect)
{
    // 1. ���������, ������ � ���� ��������
    glBindFramebuffer(GL_READ_FRAMEBUFFER, m_fbo);        // ��������
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);            // ���� � �������� FBO

    //
    // 2. �������� �������� ����������
    //
    glBlitFramebuffer(
        0, 0, m_width, m_height,      // ������� ������
        drawRect.left, drawRect.top, drawRect.Width(), drawRect.Height(),      // ������� ������
        GL_COLOR_BUFFER_BIT,      // ��� ������ ����������
        GL_NEAREST                // ���������� (����� GL_LINEAR)
    );

    //
    // 3. ���������
    //
    glBindFramebuffer(GL_READ_FRAMEBUFFER, 0);        // ��������
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);        // ���� � �����


}
