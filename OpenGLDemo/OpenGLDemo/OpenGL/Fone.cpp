#include "pch.h"
#include "Fone.h"


CFone::CFone()
    : m_initialized(false)
    , m_fbo(0)
    , m_texture(0)
{
}

CFone::~CFone()
{
    if (m_initialized)
    {
        glDeleteFramebuffers(1, &m_fbo);
        glDeleteTextures(1, &m_texture);
    }
}

bool
CFone::setup()
{

    m_textureData = (DWORD *)malloc(m_width * m_height * 4);

    if (!m_textureData)
        return false;

    memset(m_textureData, 0, m_width * m_height * 4);

    /*for (int i = 0; i < m_width; i++)
    {
        *(m_textureData + (m_width * 200) + i) = 0xFFFFFFFF;
    }*/

    // 1. Создаём текстуру
    glGenTextures(1, &m_texture);
    glBindTexture(GL_TEXTURE_2D, m_texture);

    // 2. Загружаем данные в текстуру. Можно менять в дальнейшем!
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, m_width, m_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, m_textureData);

    // 3. Настраиваем фильтрацию (обязательно!)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    // 4. Привязываем текстуру к framebuffer
    glGenFramebuffers(1, &m_fbo);
    glBindFramebuffer(GL_FRAMEBUFFER, m_fbo);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_texture, 0);

    // Проверка статуса фреймбуфера
    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
        
    }

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glBindTexture(GL_TEXTURE_2D, 0);

    m_initialized = true;
    return true;
}

bool
CFone::prepareFBOData()
{
    //
    // Пример рисования через дополнительный FBO
    //
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, m_fbo);       // цель — искуственный FBO

    //--------------------
    // РИСОВАНИЕ 
    //--------------------

    // 
    //  это рисование будет в ДОПОЛНИТЕЛЬНЫЙ БУФЕР
    //
    glClearColor(0.0f, 0.5f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glBegin(GL_LINES);

    glVertex3f(0.0f, 0.0f, 0.0f);    // начало
    glVertex3f(50.0f, 0.0f, 0.0f);  // конец

    glEnd();


    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);  // отсоединяем

    return true;
}


void
CFone::Draw(CRect &drawRect)
{
    // 1. Указываем, откуда и куда копируем
    glBindFramebuffer(GL_READ_FRAMEBUFFER, m_fbo);        // источник
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);            // цель — экранный FBO

    //
    // 2. Копируем цветовое содержимое
    //
    glBlitFramebuffer(
        0, 0, m_width, m_height,      // область чтения
        drawRect.left, drawRect.top, drawRect.Width(), drawRect.Height(),      // область записи
        GL_COLOR_BUFFER_BIT,      // что именно копировать
        GL_NEAREST                // фильтрация (можно GL_LINEAR)
    );

    //
    // 3. отключаем
    //
    glBindFramebuffer(GL_READ_FRAMEBUFFER, 0);        // источник
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);        // цель — экран


}
