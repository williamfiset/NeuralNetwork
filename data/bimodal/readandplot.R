#
# Read in the distribution and plot it
#

data <- read.table("training-raw.txt", sep=",", header=TRUE)

pdf(file="bimodalplot.pdf", height=5, width=5)


# can get the first rows via data[1:200,]
# can get the first columns via data[,1:4]
# so data[1:200,1:4] provides the first four columns of the first 200 rows

# pch=21 == plot character 21 == filled circle
pairs(data[1:400,1:4], main="Bimodal Distribution Data", pch=21,
		bg = c("red", "green3", "blue", "orange")[unclass(data$Label)])

dev.off()
