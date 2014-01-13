#include <sys/signalfd.h>
#include <signal.h>
#include <unistd.h>
#include <iostream>


int main(int argc, char *argv[])
{
  sigset_t mask;
  int sfd;
  struct signalfd_siginfo fdsi;
  ssize_t s;

  sigemptyset(&mask);
  sigaddset(&mask, SIGINT);
  sigaddset(&mask, SIGQUIT);

  if (sigprocmask(SIG_BLOCK, &mask, NULL) == -1)
  {
    return 127;
  }

  sfd = signalfd(-1, &mask, 0);
  if (sfd == -1)
  {
    return 127;
  }

  while(true)
  {
    s = read(sfd, &fdsi, sizeof(struct signalfd_siginfo));
    if (s != sizeof(struct signalfd_siginfo))
    {
      return 127;
    }

    if (fdsi.ssi_signo == SIGINT) 
    {
      std::cout << "Got SIGINT" << std::endl;
    } 
    else if (fdsi.ssi_signo == SIGQUIT) 
    {
      std::cout << "Got SIGQUIT" << std::endl;
      return 0;
    } 
    else 
    {
      std::cout << "Unexpected signal: " << fdsi.ssi_signo << std::endl;
    }
  }

  return 0;
}
