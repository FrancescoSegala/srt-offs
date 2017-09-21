#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>




typedef struct Srt{
  int offset;
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
      print_helper("too few argument: type -h or --help for help");
      return 0;
    }
    if ( strcmp("-o",argv[1])==0 && (atoi(argv[2])*atoi(argv[2])) ){
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
        SRT->offset=atoi(argv[o]);
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
  int sec_d_f=0, sec_u_f=0;//cooment
  int sec_d_to=0, sec_u_to=0;//cooment
  int offset=SRT->offset;
  int pathname_len=strlen(SRT->pathname);
  char path[pathname_len];
  int sec_from_txt,min_from_txt,hour_from_txt;
  int sec_to_txt,min_to_txt,hour_to_txt;
  int tot_sec_from=0;
  strncpy(path, SRT->pathname,pathname_len-4);//4 is for .srt

  FILE* f=fopen(SRT->pathname, "r+");
  FILE* fout=fopen(  strcat( path,"OFFSET.srt") , "w+");

  if (!f || !fout){
    print_helper("file not opened");
    return;
  }
  char line[256];
  while( fgets(line,256, f)!=NULL ){

    if(strstr(line, "-->") != NULL) {
      sec_from_txt = (line[6]-'0')*10 + (line[7]-'0');
      min_from_txt = (line[3]-'0')*10 + (line[4]-'0');
      hour_from_txt = (line[0]-'0')*10 + (line[1]-'0');
      tot_sec_from=hour_from_txt*60*60+min_from_txt*60+sec_from_txt+offset;

      if (offset<0 && tot_sec_from<0){
        print_wrong_usage("offset too negative");
        return ;
      }
      sec_to_txt=(line[23]-'0')*10 + (line[24]-'0');
      min_to_txt=(line[20]-'0')*10 + (line[21]-'0');
      hour_to_txt=(line[17]-'0')*10 + (line[18]-'0');
      if (offset+sec_from_txt > 60 ){
        min_from_txt=min_from_txt+(sec_from_txt+offset)/60;
        sec_from_txt=(sec_from_txt+offset)%60;
      }
      else if(offset+sec_to_txt > 60){
        min_from_txt=min_from_txt+(sec_from_txt+offset)/60;
        sec_from_txt=(sec_from_txt+offset)%60;
      }
      else {
      }
      //very sad thing to do
      char c[2];
      sprintf(c, "%.2d",sec_from_txt+offset);
      line[6]=c[0];
      line[7]=c[1];
      sprintf (c, "%d",min_from_txt);
      if(min_from_txt < 10){
        line[3]='0';
        line[4]=c[0];
      }
      else {
        line[3]=c[0];
        line[4]=c[1];
      }
      sprintf (c, "%.2d",hour_from_txt);
      if (hour_from_txt <10 ) {
        line[0]='0';
        line[1]=c[0];
      }
      else {
        line[0]=c[0];
        line[1]=c[1];
      }
      sprintf(c, "%.2d",sec_to_txt+offset);
      line[23]=c[0];
      line[24]=c[1];
      sprintf(c, "%.2d",min_to_txt);
      line[20]=c[0];
      line[21]=c[1];
      sprintf(c, "%.2d",hour_to_txt);
      if (hour_to_txt == 0) {
        line[17]='0';
        line[18]=c[0];
      }
      else {
        line[17]=c[0];
        line[18 ]=c[1];
      }
    }
    fprintf(fout, "%s",line );
  }
  fclose(f);
  fclose(fout);
}


int main(int argc, char const *argv[]) {
  Srt* sub=(Srt*)malloc(sizeof( Srt));
  //char path[265];
  short command_line_ok=check_command(argc,argv,sub);
  //sub->pathname=path;
  if ( command_line_ok ){
      do_job(sub);
  }
  else print_wrong_usage("Something went wrong, retry.");
  return command_line_ok;
}






/**/
