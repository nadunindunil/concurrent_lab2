
double countSamples(double mean, double std)
{
    double v = (100 * 1.96 * std) / (5 * mean);
    return v * v;
}

void printData(char *name, int n, int m, int samples, float mMember, float mInsert, float mDelete, int threads, double sum, double sqrSum)
{

    printf("%s \t n=%d, m=%d, s=%d, member=%f, insert=%f, delete=%f, threads=%d\n", name, n, m, samples, mMember, mInsert, mDelete, threads);

    double avg = sum / samples, sqavg = sqrSum / samples;
    double std = sqrt(sqavg - avg * avg);

    double samplesExpect = countSamples(avg, std);

    printf("Sum: %f\n", sum);
    printf("Std: %f\n", std);
    printf("Avg: %f\n", avg);
    printf("SquareAvg: %f\n", sqavg);
    printf("Samples count: %d\n", samples);
    printf("Samples(Needed for a 95 accuracy): %f\n\n", samplesExpect);

    FILE *f = fopen("overview.csv", "a");
    fprintf(f, "%s, %d, %d, %f, %f, %f, %d, %f, %f, %f\n", name, n, m, mMember, mInsert, mDelete, threads, samplesExpect, avg, std);
}