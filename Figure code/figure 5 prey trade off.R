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
dir3 = "C:/Users/aliss/OneDrive - University of Aberdeen/PhD in AI/evolution things preparing for maladaptation work/trade off def vs adapt"

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

create_line_plot_mutation = function(dir, filt){
  
  table = data.frame()
  dirs=as.data.frame(list.dirs(dir))
  names(dirs)="name"
  cartelle=dirs%>%filter(name %like% filt)
  
  ###COME CAZZO ACCEDO AI NUMERINI DI DISPERSAL PORCA PALETTA
  for(i in 1:dim(cartelle)[1]){
    f=list.files(cartelle$name[i],pattern = "*Pops*")
    prova=fread(paste(cartelle$name[i],f,sep = "/"))
    prova$dist0 = substring(cartelle$name[i], str_locate(cartelle$name[i],"d_0")[1,2] +2, str_locate(cartelle$name[i],"d_0")[1,2] + 4)
    table = rbind(table, prova)
  }
  
  table2 = table%>%filter(gen==2500)
  table2$dist0 = as.factor(table2$dist0)
  #this is for when you want to plot stuff normally (no discretizing theta!)
  t= table2 %>%
    group_by(dist0, y) %>%
    summarise(mean(N1) )
  names(t) = c("dist0", "y", "N1mean")
  
  if(min(t$y)==200){
    t$y= t$y -200
  }else if(min(t$y)==100){
    t$y= t$y -100
  }
  
  #this is if you need to change theta when you add environmental variation
  
  #table2$theta_new=0
  
  #for (i in 1:500){
  #  table2$theta_new[table2$theta >= 0.045*(i-1) & table2$theta <= 0.045*i]=0.045*(i-1)
  #}
  
  #t= table2 %>%
  #  group_by(dist0, theta_new) %>%
  #  summarise(mean(N0) )
  #names(t) = c("dist0", "theta_new", "N0mean")
  #it ends here
  
  
  #old line plot
  MyPlot = ggplot(data = t, aes(x=y, y=N1mean, group=dist0)) + #put theta_new here when theta discrete
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
    ylim(c(0,12.0))
  #xlim(c(11.5,12))+
  guides(fill=guide_legend(title="predator mean dispersal distance"))
  #old lineplot ends here
  
  
  #
  
  
  return(MyPlot)
  
}

#plots without env variation

#AllFixed = create_line_plot_mutation(dir0, filt1) + labs(title = "No Environmental\nvariability", 
#                                                         tag = "A") +theme(plot.title = element_text(hjust = 0.5, size = 30),
#                                                                           plot.tag = element_text(size = 22))
#OnlyNiche = create_line_plot_mutation(dir1, filt2)+ labs(tag = "B") +theme(plot.tag = element_text(size = 22))
#OnlyDefence = create_line_plot_mutation(dir0, filt3)+ labs(tag = "C") +theme(plot.tag = element_text(size = 22))
AllEvolveNoTradeOff = create_line_plot_mutation(dir2, filt4)+ labs(tag = "A") +theme(plot.tag = element_text(size = 22))
AllEvolveTradeOff = create_line_plot_mutation(dir3, filt4)+ labs(tag = "C") +theme(plot.tag = element_text(size = 22))
AllEvolveNoTradeOffEnv = create_line_plot_mutation(dir2, filt8)+ labs(tag = "B") +theme(plot.tag = element_text(size = 22))
AllEvolveTradeOffEnv = create_line_plot_mutation(dir3, filt8)+ labs(tag = "D") +theme(plot.tag = element_text(size = 22))


#plots with env variation
#AllFixedEnv = create_line_plot_mutation(dir1, filt5)+ labs(title = "With Environmental\nvariability", 
#                                                           tag = "A1") +theme(plot.title = element_text(hjust = 0.5, size = 30),
 #                                                                             plot.tag = element_text(size = 22))
#OnlyNicheEnv = create_line_plot_mutation(dir1, filt6)+ labs(tag = "B1") +theme(plot.tag = element_text(size = 22))
#OnlyDefenceEnv = create_line_plot_mutation(dir1, filt7)+ labs(tag = "C1") +theme(plot.tag = element_text(size = 22))
#AllEvolveEnv = create_line_plot_mutation(dir2, filt8)+ labs(tag = "D1") +theme(plot.tag = element_text(size = 22))


#put them all together
#let's try patchwork
wrap_plots(AllEvolveNoTradeOff, AllEvolveNoTradeOffEnv,
           AllEvolveTradeOff, AllEvolveTradeOffEnv, ncol = 2, nrow = 2) 



wrap_plots(AllFixed, AllFixedEnv, OnlyNiche, OnlyNicheEnv, OnlyDefence,
           OnlyDefenceEnv, AllEvolve, AllEvolveEnv, ncol = 2, nrow = 4) 

