

REQUIRED_CHANNELS = 3 # if patterns have less, they'll be padded with ch1
BYTE_MAX = 255
TWOBYTES_MAX = 65535
TIMING_CONVERSION_FACTOR = 1024.0 # gives a max of ~60 seconds per keyframe at 1x, and a min of ~1 ms
SHOW_PLOTS = False
outfile = 'src/patterns_generated.h'
infile = 'patterns.txt'
char_startpattern = '+'

patterns: dict[str, list[list[float]]] = {}
collecting_data = False
data_line_iterator = 0
current_pattern = ''
current_data: list[list[float]] = []

def makeCfnName(s: str) -> str:
    r: str = ''
    for c in s:
        if c.isalnum():
            r += c
    return r

with open(infile, 'rt') as file_in:
    with open(outfile, 'wt') as file_out:
        file_out.write('/* AUTOMATICALLY GENERATED FILE. DO NOT MODIFY DIRECTLY */\n')
        file_out.write('#pragma once\n')
        file_out.write('#include <Arduino.h>\n\n')
        file_out.write(f'static const double bytemax = {int(BYTE_MAX)}.0;\n')
        file_out.write(f'static const double timingConversion = {TIMING_CONVERSION_FACTOR};\n')
        file_out.write(f'static int k = 1;\n\n')

        for line in file_in.read().splitlines():
            if not line.strip() or line[0] in ['#']:
                continue
            if collecting_data and line[0] == char_startpattern:
                collecting_data = False
                patterns[current_pattern] = current_data
                current_data = []
            if collecting_data:
                current_data.append([])
                current_data[data_line_iterator] = [float(x) for x in line.strip().split(' ')]
                data_line_iterator += 1
            if line[0] == char_startpattern:
                current_pattern = makeCfnName(line)
                collecting_data = True
                data_line_iterator = 0

        if collecting_data:
            patterns[current_pattern] = current_data

        for pattern in patterns:
            patternData = patterns[pattern]
            numChannels = len(patternData) - 1
            patternLen = len(patternData[0])

            file_out.write(f'void PP_{pattern}(void(* fWrite)(uint8_t, double), void(* fDelay)(double)) {{\n')
            file_out.write(f'\tconstexpr uint32_t LENGTH = {patternLen};\n')
            for i in range(numChannels):
                file_out.write(f'\tconst uint8_t c{i+1}_x[LENGTH] = {{')
                for d in range(patternLen):
                    file_out.write(f'{int(patternData[i][d] * BYTE_MAX)}, ')
                file_out.write('};\n')
            file_out.write('\tconst uint16_t durs[LENGTH] = {')
            for d in range(patternLen):
                file_out.write(f'{int(patternData[numChannels][d] * TIMING_CONVERSION_FACTOR)}, ')
            file_out.write('};\n')

            #file_out.write('\tstatic int k = 0;\n')
            file_out.write('\tk = k % LENGTH;\n')
            for z in range(numChannels):
                file_out.write(f'\tfWrite({z+1}, c{z+1}_x[k] / bytemax);\n')
            if numChannels < REQUIRED_CHANNELS:
                for zz in range(numChannels+1, REQUIRED_CHANNELS+1):
                    file_out.write(f'\tfWrite({zz}, c{1}_x[k] / bytemax);\n')
            file_out.write('\tfDelay(durs[k] / timingConversion);\n')
            file_out.write('\tk += 1;\n')
            file_out.write('}\n')

        # footer
        file_out.write('\nvoid (*loadedPatterns[]) (void(*f)(uint8_t, double), void(* fDelay)(double)) = {\n')
        for pattern in patterns:
            file_out.write(f'\tPP_{pattern},\n')
        file_out.write('};\n')

if SHOW_PLOTS:
    NUM_PERIODS = 2
    import matplotlib.pyplot as plt
    import time
    for pattern in patterns:
        patternDataX = patterns[pattern][-1] * NUM_PERIODS
        # pre process: make t cumulative
        for i in range(1, len(patternDataX)):
            patternDataX[i] = patternDataX[i] + patternDataX[i-1]

        patternDataYs = patterns[pattern][:-1]
        plt.figure()
        plt.clf()
        plt.xlabel('t (s)')
        plt.ylabel('intensity (%)')
        plt.grid(True)
        plt.title(pattern)
        # pre process: insert square'd edges
        xs_windowed = [0.0]
        for x in range(1, len(patternDataX)):
            xs_windowed.append(patternDataX[x-1])
            xs_windowed.append(patternDataX[x])
        xs_windowed.append(xs_windowed[-1])
        legendChNo = 1
        legendStr : list[str] = []
        for ys in patternDataYs:
            ys = ys * NUM_PERIODS
            legendStr.append('CH. ' + str(legendChNo))
            legendChNo += 1
            ys_windowed = []
            for y in ys:
                ys_windowed.append(y)
                ys_windowed.append(y)
            print(len(xs_windowed))
            print(len(ys_windowed))
            print(xs_windowed)
            print(ys_windowed)
            # rotate
            endval = ys_windowed.pop(0)
            ys_windowed_rotated = ys_windowed.copy()
            ys_windowed_rotated.append(endval)

            plt.plot(xs_windowed[1:], ys_windowed_rotated[1:])
        plt.legend(legendStr)
    plt.ion()
    plt.show()
    plt.pause(30)

            
                

            



