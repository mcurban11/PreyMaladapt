library(data.table)
library(ggplot2)
library(dplyr)
library(stringr)
library(gridExtra)
library(ggpubr)
library(cowplot)
library(patchwork)
library(wesanderson)
####OLD FOLDER STRUCTURES
#dir1 = "C:/Users/aliss/OneDrive - University of Aberdeen/PhD in AI/evolution things preparing for maladaptation work/out padded land some broken sims/"

#dir1 = "C:/Users/aliss/OneDrive - University of Aberdeen/PhD in AI/evolution things preparing for maladaptation work/right bound with env for predator/"
#dir2 = "C:/Users/aliss/OneDrive - University of Aberdeen/PhD in AI/evolution things preparing for maladaptation work/outputs greta/outputs greta 05/"
#dir3 = "C:/Users/aliss/OneDrive - University of Aberdeen/PhD in AI/evolution things preparing for maladaptation work/no niche old boundary/"
#temporary fix...
#dir4 = "C:/Users/aliss/OneDrive - University of Aberdeen/PhD in AI/evolution things preparing for maladaptation work/outputs greta/outputs greta 04/"
####

#NEW FOLDER STRUCTURE
#for the case of just defence evolution, no env variability
dir0 = "C:/Users/aliss/OneDrive - University of Aberdeen/PhD in AI/evolution things preparing for maladaptation work/outputs greta/outputs greta 05/"
dir1 = "C:/Users/aliss/OneDrive - University of Aberdeen/PhD in AI/evolution things preparing for maladaptation work/padded land with niche/"
dir2 = "C:/Users/aliss/OneDrive - University of Aberdeen/PhD in AI/evolution things preparing for maladaptation work/out padded land some broken sims/"

#no env variation
filt3 = "def_evol\\[1\\]-d_0\\[.*\\]-d_1\\[400\\]-def_mean\\[0.5\\]_.*"
#filt3 = "def_evol\\[1\\]-adapt_evol\\[0\\]-d_0\\[.*\\]-d_1\\[400\\]-w_0\\[50\\]-env_var\\[0\\]-env_var_on\\[0\\]-env_mut_sd\\[0.005\\]_.*"
filt4 = "def_evol\\[1\\]-adapt_evol\\[1\\]-d_0\\[.*\\]-d_1\\[400\\]-w_0\\[5\\]-env_var\\[0\\]-env_var_on\\[0\\]-env_mut_sd\\[0.03\\]_.*"

#with env variation
#filtTemp = "def_evol\\[1\\]-d_0\\[.*\\]-d_1\\[400\\]-def_mean\\[0.5\\]_.*"
filt7 = "def_evol\\[1\\]-adapt_evol\\[0\\]-d_0\\[.*\\]-d_1\\[400\\]-w_0\\[50\\]-env_var\\[1.0\\]-env_var_on\\[1\\]-env_mut_sd\\[0.03\\]_.*"
filt8 = "def_evol\\[1\\]-adapt_evol\\[1\\]-d_0\\[.*\\]-d_1\\[400\\]-w_0\\[5\\]-env_var\\[1.0\\]-env_var_on\\[1\\]-env_mut_sd\\[0.03\\]_.*"

create_line_plot_traits_mean = function(dir, filt){
  
  table = data.frame()
  dirs=as.data.frame(list.dirs(dir))
  names(dirs)="name"
  cartelle=dirs%>%filter(name %like% filt)
  
  ###COME CAZZO ACCEDO AI NUMERINI DI DISPERSAL PORCA PALETTA
  for(i in 1:dim(cartelle)[1]){
    f=list.files(cartelle$name[i],pattern = "*Traits*")
    prova=fread(paste(cartelle$name[i],f,sep = "/"))
    prova$dist0 = substring(cartelle$name[i], str_locate(cartelle$name[i],"d_0")[1,2] +2, str_locate(cartelle$name[i],"d_0")[1,2] + 4)
    table = rbind(table, prova)
  }
  #if you want theta
  #table2 = table%>%filter(gen==2500)
  #table2$dist0 = as.factor(table2$dist0)
  #table2$theta=0.075*(300-table2$y-1)
  #t= table2 %>%
  #  group_by(theta, dist0) %>%
  #  summarise(mean(fitClim_mean) )
  #names(t) = c("theta", "dist0", "fitClim_mean")
  #
  
  #if you want y
  table3 = table%>%filter(gen==2500)
  table3$dist0 = as.factor(table3$dist0)
  #this is for when you want to plot stuff normally (no discretizing theta!)
  t2= table3 %>%
    group_by(dist0, y) %>%
    #summarise(mean(fitClim_mean) )
    #names(t2) = c("dist0", "y", "fitClim_mean")
    summarise(mean(fitClim_mean), mean(mean_def) )
  names(t2) = c("dist0", "y", "fitClim_mean", "mean_def")
  
  MyPlot = ggplot(data = t2, aes(x=y)) +
    geom_line(aes(y=fitClim_mean, group=dist0, colour=dist0),linetype = "dashed", size = 1)+
    geom_line(aes(y = mean_def, group = dist0, colour=dist0), size = 1)+
    scale_color_manual(values=c("#00CCCC", "#FF3399", "#9933FF")) +
    scale_y_continuous(
      name = "Mean\nenvironmental trait",
      sec.axis = sec_axis(~., name="mean defence")
    ) +
    theme(panel.grid.major = element_blank(), panel.grid.minor = element_blank(),panel.background = element_blank(),
          axis.line = element_line(colour = "black"), legend.position = "top", legend.title = element_blank(), text = element_text(size = 20)) +
    xlab("y") 
  
  return(MyPlot)
  
}



create_line_plot_traits_standard_deviation = function(dir, filt){
  
  table = data.frame()
  dirs=as.data.frame(list.dirs(dir))
  names(dirs)="name"
  cartelle=dirs%>%filter(name %like% filt)
  
  ###COME CAZZO ACCEDO AI NUMERINI DI DISPERSAL PORCA PALETTA
  for(i in 1:dim(cartelle)[1]){
    f=list.files(cartelle$name[i],pattern = "*Traits*")
    prova=fread(paste(cartelle$name[i],f,sep = "/"))
    prova$dist0 = substring(cartelle$name[i], str_locate(cartelle$name[i],"d_0")[1,2] +2, str_locate(cartelle$name[i],"d_0")[1,2] + 4)
    table = rbind(table, prova)
  }
  #if you want theta
  #table2 = table%>%filter(gen==2500)
  #table2$dist0 = as.factor(table2$dist0)
  #table2$theta=0.075*(300-table2$y-1)
  #t= table2 %>%
  #  group_by(theta, dist0) %>%
  #  summarise(mean(fitClim_mean) )
  #names(t) = c("theta", "dist0", "fitClim_mean")
  #
  
  #if you want y
  table3 = table%>%filter(gen==2500)
  table3$dist0 = as.factor(table3$dist0)
  #this is for when you want to plot stuff normally (no discretizing theta!)
  t2= table3 %>%
    group_by(dist0, y) %>%
    summarise(mean(fitClim_sd), mean(sd_def) )
  names(t2) = c("dist0", "y", "fitClim_sd", "sd_def")
  
  MyPlot = ggplot(data = t2, aes(x=y)) +
    geom_line(aes(y=fitClim_sd, group=dist0, colour=dist0),linetype = "dashed", size = 1)+
    geom_line(aes(y = sd_def, group = dist0, colour=dist0), size = 1)+
    scale_color_manual(values=c("#00CCCC", "#FF3399", "#9933FF")) +
    scale_y_continuous(
      name = "genetic variation\nenvironmental trait (sd)",
      sec.axis = sec_axis(~., name="genetic variation\nin defence(sd)")
    ) +
    theme(panel.grid.major = element_blank(), panel.grid.minor = element_blank(),panel.background = element_blank(),
          axis.line = element_line(colour = "black"), legend.position = "top", legend.title = element_blank(), text = element_text(size = 20)) +
    xlab("y") 
  
  return(MyPlot)
  
}

create_line_plot_prey_abundance = function(dir, filt){
  
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
    summarise(mean(N0) )
  names(t) = c("dist0", "y", "N0mean")
  
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
  MyPlot = ggplot(data = t, aes(x=y, y=N0mean, group=dist0)) + #put theta_new here when theta discrete
    #geom_point(aes(colour=disp))+
    geom_line(aes(colour=dist0), size = 1)+
    scale_color_manual(values=c("#00CCCC", "#FF3399", "#9933FF")) +
    theme(panel.grid.major = element_blank(), panel.grid.minor = element_blank(),
          panel.background = element_blank(), axis.line = element_line(colour = "black"), 
          legend.position = "top", legend.title = element_blank(), text = element_text(size = 20)) +
    xlab("y") + ylab("prey mean\nabundance") +
    ylim(c(65.0,95.0))+
    guides(fill=guide_legend(title="predator mean dispersal distance"))
  #old lineplot ends here
  
  return(MyPlot)
  
}

PreyAbundanceFixedDefence = create_line_plot_prey_abundance(dir0, filt3)+ labs(title = "No Environmental\nvariability", 
                                                                               tag = "A") +theme(plot.title = element_text(hjust = 0.5, size = 30),
                                                                                                 plot.tag = element_text(size = 22))
PreyAbundance = create_line_plot_prey_abundance(dir1, filt4)+ labs(tag = "B") +theme(plot.tag = element_text(size = 22))
PreyMeanTraits = create_line_plot_traits_mean(dir1, filt4)+ labs(tag = "C") +theme(plot.tag = element_text(size = 22))
PreyStandardDeviationTraits = create_line_plot_traits_standard_deviation(dir1, filt4)+ labs(tag = "D") +theme(plot.tag = element_text(size = 22))

#with env variation

PreyAbundanceFixedDefenceEnv = create_line_plot_prey_abundance(dir2, filt7)+ labs(title = "With Environmental\nvariability", 
                                                                                  tag = "A1") +theme(plot.title = element_text(hjust = 0.5, size = 30),
                                                                                                     plot.tag = element_text(size = 22))
PreyAbundanceEnv = create_line_plot_prey_abundance(dir1, filt8)+ labs(tag = "B1") +theme(plot.tag = element_text(size = 22))
PreyMeanTraitsEnv = create_line_plot_traits_mean(dir1, filt8)+ labs(tag = "C1") +theme(plot.tag = element_text(size = 22))
PreyStandardDeviationTraitsEnv = create_line_plot_traits_standard_deviation(dir1, filt8)+ labs(tag = "D1") +theme(plot.tag = element_text(size = 22))


#do I want four plots or just three
wrap_plots(PreyAbundanceFixedDefence, PreyAbundanceFixedDefenceEnv, PreyAbundance,PreyAbundanceEnv,
           PreyMeanTraits, PreyMeanTraitsEnv, PreyStandardDeviationTraits,PreyStandardDeviationTraitsEnv,
           ncol = 2, nrow = 4)

#I think I want just three
#wrap_plots(PreyAbundance,PreyAbundanceEnv,
#           PreyMeanTraits, PreyMeanTraitsEnv, PreyStandardDeviationTraits,PreyStandardDeviationTraitsEnv,
#           ncol = 2, nrow = 3)
