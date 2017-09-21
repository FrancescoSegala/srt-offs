#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>




typedef struct Srt{
  float offset;
  const char* pathname;
}Srt;


void print_helper(char* message){
  printf("%s\n",message );
}

void print_wrong_usage(char* message){
  printf("WRONG USAGE!\nusage:  srtresync [-o offset] file\nor use: srtresync file [-o offset]\nERROR MESSAGE:%s\n",message );
}


/*
this fun che the command line integrity and assign the struct* Srt
with the correct values.
*/
int check_command(int argc,const char* argv[], struct Srt* SRT ){
    int o=0,f=3;
    if (argc <= 2 ){
      print_helper("type -h or --help for help");
      return 0;
    }
    if ( strcmp("-o",argv[1])==0 && (atof(argv[2])*atof(argv[2])) ){
      o=2;
      f=3;
    }
    else if(strcmp("-o",argv[2])==0 && (atof(argv[3])*atof(argv[3])) ){
      o=3;
      f=1;
    }
    else if (argc==2 && (strcmp("-h",argv[1])==0 || strcmp("--help",argv[1])==0 ) ){
      print_helper("this program ....bla");
      return 0;
    }
    else{
      print_helper("usage :srtresync [-o offset] file\nsrtresync file [-o usage]");
      return 0;
    }

    if ( o ) {
      char cwd[1024];
      if (getcwd(cwd, sizeof(cwd)) == NULL ){
        print_wrong_usage("getcwd == NULL");
        return 0;
      }
      if(argc>1 && access( argv[f], F_OK ) != -1 ) {
        SRT->pathname=argv[f];
        SRT->offset=atof(argv[o]);
        return 1;
      }
      else {
        print_wrong_usage("file not exist\n:(\n");
        return 0;
      }
    }
      print_wrong_usage("input malformed");
      return 0;
  }



void do_job(struct Srt* SRT){
  int sec_d=0, sec_u=0,min_d=0,min_u=0,h_d=0,h_u=0;//cooment

  FILE* f=fopen(SRT->pathname, "r+");
  if (!f){
    print_helper("file not opened");
    return;
  }
  char line[256];
  while( fgets(line,256, f)!=NULL ){

    if(strstr(line, "-->") != NULL) {
        float sec_from=(line[6]-'0')*10 + (line[7]-'0');
        float sec_to=(line[23]-'0')*10 + (line[24]-'0');
        printf("%f   %f %f\n",SRT->offset,sec_from,sec_to );
        printf("%f   %f %f\n",SRT->offset,sec_from+SRT->offset,sec_to+SRT->offset );
        printf("%c\n",'21'+'3'-'0' );
    }
      scanf("%s\n",line );
  }
}


int main(int argc, char const *argv[]) {
  Srt* sub=(Srt*)malloc(sizeof( Srt));
  char path[265];
  sub->pathname=path;
  short command_line_ok=check_command(argc,argv,sub);
  if ( command_line_ok ){
      do_job(sub);
  }
  else print_wrong_usage("Something went wrong, retry.");
  return command_line_ok;
}






/**/
