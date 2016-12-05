
rm(list=ls())

setwd("~/Desktop/GitHub/NeuralNetwork/collected_data/hidden_nodes")

df <- read.csv("hiddennodes_bimodal_data.csv", sep = ",")
df$Mean
df$Median
df$Neurons

# Histogram Colored (blue and red)
barplot(df$Mean, col=rgb(1,0,0,0.5),xlim=c(0,22), ylim=c(0,1.0), main="Bimodal number of hidden nodes", xlab="Number of hidden nodes", ylab = "Percentage of tests passed")
barplot(df$Median, col=rgb(0,0,1,0.5), add=T)

legend("topleft", c("A", "B"), cex = 1.3, fill=c(rgb(1,0,0,0.5), rgb(0,0,1,0.5)))

box()
