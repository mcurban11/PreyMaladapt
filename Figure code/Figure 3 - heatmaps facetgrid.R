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

####OLD FOLDER STRUCTURE
#dir1 = "C:/Users/aliss/OneDrive - University of Aberdeen/PhD in AI/evolution things preparing for maladaptation work/right bound with env for predator/"
#dir2 = "C:/Users/aliss/OneDrive - University of Aberdeen/PhD in AI/evolution things preparing for maladaptation work/no niche old boundary/"
#for the case of fixed defence and just defence evolution, no env variability
#dir3 = "C:/Users/aliss/OneDrive - University of Aberdeen/PhD in AI/evolution things preparing for maladaptation work/outputs greta/outputs greta 05/"

###NEW FOLDERS
#for the case of fixed defence and just defence evolution, no env variability
dir0 = "C:/Users/aliss/OneDrive - University of Aberdeen/PhD in AI/evolution things preparing for maladaptation work/outputs greta/outputs greta 05/"

dir1 = "C:/Users/aliss/OneDrive - University of Aberdeen/PhD in AI/evolution things preparing for maladaptation work/out padded land some broken sims/"
dir2 = "C:/Users/aliss/OneDrive - University of Aberdeen/PhD in AI/evolution things preparing for maladaptation work/padded land with niche/"
###

##no environmental variation and weak selection
filt1 = "def_evol\\[0\\]-d_0\\[400\\]-d_1\\[400\\]-def_mean\\[1.0\\]_.*"
filt2 = "def_evol\\[0\\]-adapt_evol\\[1\\]-d_0\\[400\\]-d_1\\[400\\]-w_0\\[5\\]-env_var\\[0\\]-env_var_on\\[0\\]-env_mut_sd\\[0.03\\]_.*"
filt3 = "def_evol\\[1\\]-d_0\\[400\\]-d_1\\[400\\]-def_mean\\[0.5\\]_.*"
filt4 = "def_evol\\[1\\]-adapt_evol\\[1\\]-d_0\\[400\\]-d_1\\[400\\]-w_0\\[5\\]-env_var\\[0\\]-env_var_on\\[0\\]-env_mut_sd\\[0.03\\]_.*"

##with environmental variation and weak selection

filt5 = "def_evol\\[0\\]-adapt_evol\\[0\\]-d_0\\[400\\]-d_1\\[400\\]-w_0\\[50\\]-env_var\\[1.0\\]-env_var_on\\[1\\]-env_mut_sd\\[0.03\\]_.*"
filt6 = "def_evol\\[0\\]-adapt_evol\\[1\\]-d_0\\[400\\]-d_1\\[400\\]-w_0\\[5\\]-env_var\\[1.0\\]-env_var_on\\[1\\]-env_mut_sd\\[0.03\\]_.*"
filt7 = "def_evol\\[1\\]-adapt_evol\\[0\\]-d_0\\[400\\]-d_1\\[400\\]-w_0\\[50\\]-env_var\\[1.0\\]-env_var_on\\[1\\]-env_mut_sd\\[0.03\\]_.*"
filt8 = "def_evol\\[1\\]-adapt_evol\\[1\\]-d_0\\[400\\]-d_1\\[400\\]-w_0\\[5\\]-env_var\\[1.0\\]-env_var_on\\[1\\]-env_mut_sd\\[0.03\\]_.*"

organise_data = function(dir, filt){
  table = data.frame()
  dirs=as.data.frame(list.dirs(dir))
  names(dirs)="name"
  #cartelle=dirs%>%filter(name %like% "adapt_evol\\[0\\]-dist_0\\[.*\\]-dist_1\\[400\\]-width_0\\[5\\]_.*")
  #the file name is different
  cartelle=dirs%>%filter(name %like% filt)
  
  for(i in 1:dim(cartelle)[1]){
    f=list.files(cartelle$name[i],pattern = "*Pops*")
    prova=fread(paste(cartelle$name[i],f,sep = "/"))
    #prova$width0 = substring(cartelle$name[i], str_locate(cartelle$name[i],"width_0")[1,2] +2, str_locate(cartelle$name[i],"width_0")[1,2] + 2)
    table = rbind(table, prova)
  }
  
  if(min(table$y)==200){
    table$y= table$y -200
  }else if(min(table$y)==100){
    table$y= table$y -100
  }
  
  return(table)
}
#table no environmental variation
AllFixed = organise_data(dir0, filt1)
AllFixed$evolution = as.factor("Fixed Traits")
OnlyNiche = organise_data(dir1, filt2)
OnlyNiche$evolution = as.factor("Just Environmental\nAdaptation")
OnlyDefence = organise_data(dir0, filt3)
OnlyDefence$evolution = as.factor("Just Defence\nEvolution")
AllEvolve = organise_data(dir2, filt4)
AllEvolve$evolution = as.factor("Both traits\nevolving")

TableNoEnvironment = rbind(AllFixed, OnlyNiche, OnlyDefence, AllEvolve)
TableNoEnvironment$environment = as.factor("No local\nheterogeneity")


#table with env variation
AllFixedEnv = organise_data(dir1, filt5)
AllFixedEnv$evolution = as.factor("Fixed Traits")
OnlyNicheEnv = organise_data(dir1, filt6)
OnlyNicheEnv$evolution = as.factor("Just Environmental\nAdaptation")
OnlyDefenceEnv = organise_data(dir1, filt7)
OnlyDefenceEnv$evolution = as.factor("Just Defence\nEvolution")
AllEvolveEnv = organise_data(dir2, filt8)
AllEvolveEnv$evolution = as.factor("Both traits\nevolving")

TableWithEnvironment = rbind(AllFixedEnv, OnlyNicheEnv, OnlyDefenceEnv, AllEvolveEnv)
TableWithEnvironment$environment = as.factor("With local\nheterogeneity")

TableFinal = rbind(TableNoEnvironment, TableWithEnvironment)

t= TableFinal %>%
  filter(gen>100) %>%
  group_by(y,gen, environment, evolution) %>%
  summarise(PredatorMean = mean(N1) )


t$PredatorMean[t$PredatorMean == 0] = NA

#to try geom raster
t$gen = as.factor(t$gen)

pal <- wes_palette("Zissou1", 100, type = "continuous")
#pal = wes_palette("Zissou1", 10, type = "continuous")
#pal = 
hex <- c("#FF0000", "#FFA500", "#FFFF00", "#008000", "#9999ff", "#000066")
#my own palette, keep playing with this...
#hex2 = c("#FF0099", "#FF6600", "#FFFF90", "#339900", "#006666", "#0000FF")

MyPlot = ggplot(t, aes(gen, y)) +
  geom_tile(aes(fill = PredatorMean),alpha=0.6, interpolate = TRUE, hjust = 1) +
  #geom_raster(aes(fill = PredatorMean),alpha=0.6, interpolate = TRUE, hjust = 1) +
  #scale_fill_gradient2(low="yellow", mid="blue", high="red", limits = c(0.0001,2.6)) +
  scale_x_discrete("Years", breaks = c("500", "1000", "1500", "2000", "2500","2600", "3000", "3500", "4000", "4500", "5000"))+
  # scale_fill_gradientn( colors = pal, , limits = c(0.0,10.6), na.value="white") +
  #scale_fill_gradientn( colors = rainbow(6), limits = c(0.0,10.6), na.value="white") +
  # scale_fill_gradientn(colors = hcl.colors(20, "RdYlGn"),limits = c(0.0,10.6), na.value="white")+
  #scale_fill_gradientn(colors = hcl.colors(20, "Spectral"))+
  #i probably want the one below (was rev(hex) before)
  scale_fill_gradientn(colours = rev(hex), na.value="white") +
  theme(
    panel.background = element_rect(fill='transparent',color="black"),
    plot.background = element_rect(fill='transparent', color=NA),
    panel.grid.major = element_blank(),
    panel.grid.minor = element_blank(),
    legend.background = element_rect(fill='transparent'),
    #legend.box.background = element_rect(fill='transparent'),
    legend.key.height= unit(0.6, 'cm'),
    legend.key.width= unit(0.4, 'cm'),
    legend.text = element_text(size = 7),
    legend.title = element_text(size = 8)
    #axis.text.x = element_blank()
    #axis.text.x = element_text(angle = 90, vjust = 0.5, hjust=1) THIS IS TO MAKE THEM VERTICAL
  ) + labs(fill='Mean Predator\nAbundance',) 

MyPlot + facet_grid(rows=vars(evolution), cols = vars(environment), scales = "free_y")

ggsave(filename = "Fig.6-PredatorHeatmap-weakSelection.tiff",path = "C:/Users/aliss/OneDrive - University of Aberdeen/PhD in AI/evolution things preparing for maladaptation work/first draft/high res figures supp/", width = 2800, height = 2500, units = "px", device='tiff', dpi=300)


