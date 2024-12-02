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

#if you have to check with old outputs folders
#dir1 = "C:/Users/aliss/OneDrive - University of Aberdeen/PhD in AI/evolution things preparing for maladaptation work/right bound with env for predator/"
#dir2 = "C:/Users/aliss/OneDrive - University of Aberdeen/PhD in AI/evolution things preparing for maladaptation work/no niche old boundary/"

#for the case of fixed defence and just defence evolution, no env variability
#dir0 = "C:/Users/aliss/OneDrive - University of Aberdeen/PhD in AI/evolution things preparing for maladaptation work/outputs greta/outputs greta 05/"

#new folders
#for the case of fixed defence and just defence evolution, no env variability
dir0 = "C:/Users/aliss/OneDrive - University of Aberdeen/PhD in AI/evolution things preparing for maladaptation work/outputs greta/outputs greta 05/"

dir1 = "C:/Users/aliss/OneDrive - University of Aberdeen/PhD in AI/evolution things preparing for maladaptation work/out padded land some broken sims/"
dir2 = "C:/Users/aliss/OneDrive - University of Aberdeen/PhD in AI/evolution things preparing for maladaptation work/padded land with niche/"

##no environmental variation and weak selection
filt1 = "def_evol\\[0\\]-d_0\\[.*\\]-d_1\\[400\\]-def_mean\\[1.0\\]_.*"
#filt1 = "def_evol\\[0\\]-adapt_evol\\[0\\]-d_0\\[.*\\]-d_1\\[400\\]-w_0\\[50\\]-env_var\\[0\\]-env_var_on\\[0\\]-env_mut_sd\\[0.03\\]_.*"
filt2 = "def_evol\\[0\\]-adapt_evol\\[1\\]-d_0\\[.*\\]-d_1\\[400\\]-w_0\\[5\\]-env_var\\[0\\]-env_var_on\\[0\\]-env_mut_sd\\[0.03\\]_.*"
filt3 = "def_evol\\[1\\]-d_0\\[.*\\]-d_1\\[400\\]-def_mean\\[0.5\\]_.*"
#filt3 = "def_evol\\[1\\]-adapt_evol\\[0\\]-d_0\\[.*\\]-d_1\\[400\\]-w_0\\[50\\]-env_var\\[0\\]-env_var_on\\[0\\]-env_mut_sd\\[0.03\\]_.*"
filt4 = "def_evol\\[1\\]-adapt_evol\\[1\\]-d_0\\[.*\\]-d_1\\[400\\]-w_0\\[5\\]-env_var\\[0\\]-env_var_on\\[0\\]-env_mut_sd\\[0.03\\]_.*"

##with environmental variation and weak selection

filt5 = "def_evol\\[0\\]-adapt_evol\\[0\\]-d_0\\[.*\\]-d_1\\[400\\]-w_0\\[50\\]-env_var\\[1.0\\]-env_var_on\\[1\\]-env_mut_sd\\[0.03\\]_.*"
filt6 = "def_evol\\[0\\]-adapt_evol\\[1\\]-d_0\\[.*\\]-d_1\\[400\\]-w_0\\[5\\]-env_var\\[1.0\\]-env_var_on\\[1\\]-env_mut_sd\\[0.03\\]_.*"
filt7 = "def_evol\\[1\\]-adapt_evol\\[0\\]-d_0\\[.*\\]-d_1\\[400\\]-w_0\\[50\\]-env_var\\[1.0\\]-env_var_on\\[1\\]-env_mut_sd\\[0.03\\]_.*"
filt8 = "def_evol\\[1\\]-adapt_evol\\[1\\]-d_0\\[.*\\]-d_1\\[400\\]-w_0\\[5\\]-env_var\\[1.0\\]-env_var_on\\[1\\]-env_mut_sd\\[0.03\\]_.*"

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
   geom_line(aes(colour=dist0), size = 0.5)+
    #scale_color_manual(values=c("#339900", "#FF3399", "#0066CC")) +
    #scale_color_manual(values=c("#00FFFF", "#FF3399", "#9933FF")) +
    scale_color_manual(values=c("#00CCCC", "#FF3399", "#9933FF")) +
    #scale_colour_brewer(palette = "PiYG")+
    theme(panel.grid.major = element_blank(), panel.grid.minor = element_blank(),
         panel.background = element_blank(), axis.line = element_line(colour = "black"), 
          legend.position = "top", legend.title = element_blank(), legend.key.size = unit(0.3, 'cm'), text = element_text(size = 10)) +
    xlab("y") + ylab("Predator mean\nabundance") +
    ylim(c(0,3.0))+
  xlim(c(0,250))+
  guides(fill=guide_legend(title="predator mean dispersal distance"))
  #old lineplot ends here
  
  
  #
  
  
  return(MyPlot)
  
}

#plots without env variation

AllFixed = create_line_plot_mutation(dir0, filt1) + labs(title = "No local\nheterogeneity", 
                                                    tag = "A") +theme(plot.title = element_text(hjust = 0.5, size = 12),
                                                                      plot.tag = element_text(size = 10))
OnlyNiche = create_line_plot_mutation(dir1, filt2)+ labs(tag = "B") +theme(plot.tag = element_text(size = 10))
OnlyDefence = create_line_plot_mutation(dir0, filt3)+ labs(tag = "C") +theme(plot.tag = element_text(size = 10))
AllEvolve = create_line_plot_mutation(dir2, filt4)+ labs(tag = "D") +theme(plot.tag = element_text(size = 10))


#plots with env variation
AllFixedEnv = create_line_plot_mutation(dir1, filt5)+ labs(title = "With local\nheterogeneity", 
                                                           tag = "A1") +theme(plot.title = element_text(hjust = 0.5, size = 12),
                                                                             plot.tag = element_text(size = 10))
OnlyNicheEnv = create_line_plot_mutation(dir1, filt6)+ labs(tag = "B1") +theme(plot.tag = element_text(size = 10))
OnlyDefenceEnv = create_line_plot_mutation(dir1, filt7)+ labs(tag = "C1") +theme(plot.tag = element_text(size = 10))
AllEvolveEnv = create_line_plot_mutation(dir2, filt8)+ labs(tag = "D1") +theme(plot.tag = element_text(size = 10))


#put them all together
#let's try patchwork
wrap_plots(AllFixed, AllFixedEnv, OnlyNiche, OnlyNicheEnv, OnlyDefence,
           OnlyDefenceEnv, AllEvolve, AllEvolveEnv, ncol = 2, nrow = 4) 


ggsave(filename = "Fig.S5-PredatorAbundance-weakSelection.tiff",path = "C:/Users/aliss/OneDrive - University of Aberdeen/PhD in AI/evolution things preparing for maladaptation work/first draft/high res figures supp/", width = 1600, height = 2500, units = "px", device='tiff', dpi=300)


#this looks shit
#title <- ggdraw() + draw_label("Predator mean abundance", fontface = 'bold')
#first_row <- plot_grid(AllFixed, AllFixedEnv,  ncol = 2, labels =c("A", "A1"))
#second_row <- plot_grid(OnlyNiche, OnlyNicheEnv, ncol = 2, labels = c("B", "B1"))
#third_row <- plot_grid(OnlyDefence, OnlyDefenceEnv, ncol = 2, labels = c("C", "C1"))
#fourth_row <- plot_grid(AllEvolve, AllEvolveEnv, ncol = 2, labels = c("D", "D1"))
#plot_grid(title, first_row, second_row, third_row, fourth_row, nrow = 5, rel_heights = c(0.2, 1, 1) )

