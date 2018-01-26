#include <unistd.h>

#include <fcntl.h>
#include <stdio.h>
#include <string>

#define STD_OUT_FD (fileno(stdout))
#define STD_ERR_FD (fileno(stderr))

class StdCapture
{
public:
    static void Init()
    {
        // make stdout & stderr streams unbuffered
        // so that we don't need to flush the streams
        // before capture and after capture 
        // (fflush can cause a deadlock if the stream is currently being 
        setvbuf(stdout,NULL,_IONBF,0);
        setvbuf(stderr,NULL,_IONBF,0);
    }

    static void BeginCapture()
    {
        if (m_capturing)
            return;

        // secure_pipe(m_pipe);
        pipe(m_pipe);
        m_oldStdOut = dup(STD_OUT_FD);
        m_oldStdErr = dup(STD_ERR_FD);
        // secure_dup2(m_pipe[WRITE],STD_OUT_FD);
        // secure_dup2(m_pipe[WRITE],STD_ERR_FD);
        dup2(m_pipe[WRITE],STD_OUT_FD);
        dup2(m_pipe[WRITE],STD_ERR_FD);
        m_capturing = true;
        // secure_close(m_pipe[WRITE]);
        close(m_pipe[WRITE]);
    }
    static bool IsCapturing()
    {
        return m_capturing;
    }
    static bool EndCapture()
    {
        if (!m_capturing)
            return false;

        m_captured.clear();
        
        // secure_dup2(m_oldStdOut, STD_OUT_FD);
        // secure_dup2(m_oldStdErr, STD_ERR_FD);
        dup2(m_oldStdOut, STD_OUT_FD);
        dup2(m_oldStdErr, STD_ERR_FD);

        const int bufSize = 1025;
        char buf[bufSize];
        int bytesRead = 0;
        bool fd_blocked(false);
        do
        {
            bytesRead = 0;
            fd_blocked = false;
            bytesRead = read(m_pipe[READ], buf, bufSize-1);

            if (bytesRead > 0)
            {
                buf[bytesRead] = 0;
                m_captured += buf;
            }
            else if (bytesRead < 0)
            {
                //fd_blocked = (errno == EAGAIN || errno == EWOULDBLOCK || errno == EINTR);
                // if (fd_blocked)
                //     std::this_thread::sleep_for(std::chrono::milliseconds(10));
            }
        }
        while( bytesRead == (bufSize-1));

        // secure_close(m_oldStdOut);
        // secure_close(m_oldStdErr);
        // secure_close(m_pipe[READ]);
        close(m_oldStdOut);
        close(m_oldStdErr);
        close(m_pipe[READ]);

        m_capturing = false;
        return true;
    }

    static std::string GetCapture()
    {
        return m_captured;
    }
private:
    enum PIPES { READ, WRITE };

    // int StdCapture::secure_dup(int src)
    // {
    //     int ret = -1;
    //     bool fd_blocked = false;
    //     do
    //     {
    //          ret = dup(src);
    //         //  fd_blocked = (errno == EINTR ||  errno == EBUSY);
    //         //  if (fd_blocked)
    //         //     std::this_thread::sleep_for(std::chrono::milliseconds(10));
    //     }
    //     while (ret < 0);
    //     return ret;
    // }
    // void StdCapture::secure_pipe(int * pipes)
    // {
    //     int ret = -1;
    //     bool fd_blocked = false;
    //     do
    //     {
    //         ret = pipe(pipes) == -1;
    //         // fd_blocked = (errno == EINTR ||  errno == EBUSY);
    //         // if (fd_blocked)
    //         //     std::this_thread::sleep_for(std::chrono::milliseconds(10));
    //     }
    //     while (ret < 0);
    // }
    // void StdCapture::secure_dup2(int src, int dest)
    // {
    //     int ret = -1;
    //     bool fd_blocked = false;
    //     do
    //     {
    //          ret = dup2(src,dest);
    //         // fd_blocked = (errno == EINTR ||  errno == EBUSY);
    //         //  if (fd_blocked)
    //         //     std::this_thread::sleep_for(std::chrono::milliseconds(10));
    //     }
    //     while (ret < 0);
    // }

    // void StdCapture::secure_close(int & fd)
    // {
    //     int ret = -1;
    //     bool fd_blocked = false;
    //     do
    //     {
    //          ret = close(fd);
    //         //fd_blocked = (errno == EINTR);
    //         //  if (fd_blocked)
    //         //     std::this_thread::sleep_for(std::chrono::milliseconds(10));
    //     }
    //     while (ret < 0);

    //     fd = -1;
    // }

    static int m_pipe[2];
    static int m_oldStdOut;
    static int m_oldStdErr;
    static bool m_capturing;

    static std::string m_captured;
};

// actually define vars.
int StdCapture::m_pipe[2];
int StdCapture::m_oldStdOut;
int StdCapture::m_oldStdErr;
bool StdCapture::m_capturing;
std::string StdCapture::m_captured;