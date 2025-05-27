library(data.table)
library(ggplot2)
library(dplyr)
library(stringr)
library(gridExtra)
library(ggpubr)
library(cowplot)
library(patchwork)
library(RColorBrewer)
library(wesanderson)



dir0 = "C:/Users/aliss/OneDrive - University of Aberdeen/PhD in AI/evolution things preparing for maladaptation work/outputs greta/outputs greta 05/"

dir1 = "C:/Users/aliss/OneDrive - University of Aberdeen/PhD in AI/evolution things preparing for maladaptation work/out padded land some broken sims/"
dir2 = "C:/Users/aliss/OneDrive - University of Aberdeen/PhD in AI/evolution things preparing for maladaptation work/padded land with niche/"

##no environmental variation and weak selection
filt1 = "def_evol\\[0\\]-d_0\\[.*\\]-d_1\\[400\\]-def_mean\\[1.0\\]_.*"
#filt1 = "def_evol\\[0\\]-adapt_evol\\[0\\]-d_0\\[.*\\]-d_1\\[400\\]-w_0\\[50\\]-env_var\\[0\\]-env_var_on\\[0\\]-env_mut_sd\\[0.03\\]_.*"
filt2 = "def_evol\\[0\\]-adapt_evol\\[1\\]-d_0\\[.*\\]-d_1\\[400\\]-w_0\\[1\\]-env_var\\[0\\]-env_var_on\\[0\\]-env_mut_sd\\[0.03\\]_.*"
filt3 = "def_evol\\[1\\]-d_0\\[.*\\]-d_1\\[400\\]-def_mean\\[0.5\\]_.*"
#filt3 = "def_evol\\[1\\]-adapt_evol\\[0\\]-d_0\\[.*\\]-d_1\\[400\\]-w_0\\[50\\]-env_var\\[0\\]-env_var_on\\[0\\]-env_mut_sd\\[0.03\\]_.*"
filt4 = "def_evol\\[1\\]-adapt_evol\\[1\\]-d_0\\[.*\\]-d_1\\[400\\]-w_0\\[1\\]-env_var\\[0\\]-env_var_on\\[0\\]-env_mut_sd\\[0.03\\]_.*"

##with environmental variation and weak selection

filt5 = "def_evol\\[0\\]-adapt_evol\\[0\\]-d_0\\[.*\\]-d_1\\[400\\]-w_0\\[50\\]-env_var\\[1.0\\]-env_var_on\\[1\\]-env_mut_sd\\[0.03\\]_.*"
filt6 = "def_evol\\[0\\]-adapt_evol\\[1\\]-d_0\\[.*\\]-d_1\\[400\\]-w_0\\[1\\]-env_var\\[1.0\\]-env_var_on\\[1\\]-env_mut_sd\\[0.03\\]_.*"
filt7 = "def_evol\\[1\\]-adapt_evol\\[0\\]-d_0\\[.*\\]-d_1\\[400\\]-w_0\\[50\\]-env_var\\[1.0\\]-env_var_on\\[1\\]-env_mut_sd\\[0.03\\]_.*"
filt8 = "def_evol\\[1\\]-adapt_evol\\[1\\]-d_0\\[.*\\]-d_1\\[400\\]-w_0\\[1\\]-env_var\\[1.0\\]-env_var_on\\[1\\]-env_mut_sd\\[0.03\\]_.*"

create_line_plot_range = function(dir, filt, dir1, filt1){
  
  table = data.frame()
  dirs=as.data.frame(list.dirs(dir0))
  names(dirs)="name"
  cartelle=dirs%>%filter(name %like% filt1)
  
  
  for(i in 1:dim(cartelle)[1]){
    f=list.files(cartelle$name[i],pattern = "*Pops*")
    prova=fread(paste(cartelle$name[i],f,sep = "/"))
    prova$dist0 = substring(cartelle$name[i], str_locate(cartelle$name[i],"d_0")[1,2] +2, str_locate(cartelle$name[i],"d_0")[1,2] + 4)
    table = rbind(table, prova)
  }
  
  table1 = table%>%filter(gen==2500)
  table1$dist0 = as.factor(table1$dist0)
  #this is for when you want to plot stuff normally (no discretizing theta!)
  t= table1 %>%
    group_by(dist0, y) %>%
    summarise(mean(N1) )
  names(t) = c("dist0", "y", "N1mean")
  
  #seconda cartella
  table2 = data.frame()
  dirs=as.data.frame(list.dirs(dir2))
  names(dirs)="name"
  cartelle=dirs%>%filter(name %like% filt4)
  
  
  for(i in 1:dim(cartelle)[1]){
    f=list.files(cartelle$name[i],pattern = "*Pops*")
    prova2=fread(paste(cartelle$name[i],f,sep = "/"))
    prova2$dist0 = substring(cartelle$name[i], str_locate(cartelle$name[i],"d_0")[1,2] +2, str_locate(cartelle$name[i],"d_0")[1,2] + 4)
    table2 = rbind(table2, prova2)
  }
  
  table3 = table2%>%filter(gen==2500)
  table3$dist0 = as.factor(table3$dist0)
  #this is for when you want to plot stuff normally (no discretizing theta!)
  t1= table3 %>%
    group_by(dist0, y) %>%
    summarise(mean(N1) )
  names(t1) = c("dist1", "y1", "N1mean_evolving")
  
  if(min(t1$y1)==200){
    t1$y1= t1$y1 -200
  }else if(min(t1$y1)==100){
    t1$y1= t1$y1 -100
  }
  
  t1 = subset(t1, y1<300)
  #calculate range
  t_range_1 = t1%>%filter(N1mean_evolving>=1)
  t_range_stat=  t_range_1 %>%
    group_by(dist1) %>%
    summarise(min_y1 = mean(min(y1)), max_y1 = mean(max(y1)))
  
  
  t_range_15 = t1%>%filter(N1mean_evolving>=1.5)
  t_range_stat=  t_range_15 %>%
    group_by(dist1) %>%
    summarise(min_y15 = mean(min(y1)), max_y15 = mean(max(y1)))
  
  #metti le tabelle insieme per calcolare la differenza
  tabella_fusa = cbind(t,t1)
  tabella_fusa$differenza =  tabella_fusa$N1mean_evolving -tabella_fusa$N1mean
  
  
  #old line plot
  MyPlot = ggplot(data = tabella_fusa, aes(x=y, y=differenza, group=dist0)) + #put theta_new here when theta discrete
    #geom_point(aes(colour=disp))+
    geom_line(aes(colour=dist0), size = 1)+
    #scale_color_manual(values=c("#339900", "#FF3399", "#0066CC")) +
    #scale_color_manual(values=c("#00FFFF", "#FF3399", "#9933FF")) +
    scale_color_manual(values=c("#00CCCC", "#FF3399", "#9933FF")) +
    #scale_colour_brewer(palette = "PiYG")+
    theme(panel.grid.major = element_blank(), panel.grid.minor = element_blank(),
          panel.background = element_blank(), axis.line = element_line(colour = "black"), 
          legend.position = "top", legend.title = element_blank(), text = element_text(size = 20)) +
    xlab("y") + ylab("Predator mean\nabundance") +
    geom_vline(data=t_range_stat, aes(xintercept = min_y15, color = dist1),size = 1)+
    geom_vline(data=t_range_stat, aes(xintercept = max_y15, color = dist1),size = 1)+
    #how do i get rid of the vertical line in the fucking legend
    # ylim(c(0,3.0))
  #xlim(c(11.5,12))+
  guides(fill=guide_legend(title="predator mean dispersal distance"))
  #old lineplot ends here
  
  
  #
  
  
  return(MyPlot)
  
}

Difference_no_env_variation = create_line_plot_range(dir0, filt3, dir2, filt4) 
#AllEvolve = create_line_plot_mutation(dir2, filt4)+ labs(tag = "D") +theme(plot.tag = element_text(size = 22))
