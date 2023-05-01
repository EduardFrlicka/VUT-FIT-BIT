import matplotlib.pyplot as plt
from scipy import signal
import numpy as np
from scipy.io.wavfile import read,write
from func import get_most_alike_samples, wave_to_frames, center_clipping, autocorelation_P

RATE = 16000

maskon_wave = np.array(read("../audio/maskon_tone.wav")[1],dtype=float)
maskoff_wave = np.array(read("../audio/maskoff_tone.wav")[1],dtype=float)
maskoff_sentence = np.array(read("../audio/maskoff_sentence.wav")[1],dtype=float)
maskon_sentence = np.array(read("../audio/maskon_sentence.wav")[1],dtype=float)


maskon_sample, maskoff_sample = get_most_alike_samples(maskon_wave, maskoff_wave, 1010, 100, RATE)

maskon_sample -= np.mean(maskon_sample)
maskoff_sample -= np.mean(maskoff_sample)

maskon_sample /= np.abs(maskon_sample).max()
maskoff_sample /= np.abs(maskoff_sample).max()

frames_maskon = wave_to_frames(maskon_sample,20,10,RATE)
frames_maskoff = wave_to_frames(maskoff_sample,20,10,RATE)

plt.figure(figsize=(12, 4))
plt.plot(frames_maskon[52])
plt.title("Ramec s ruškom")
plt.xlabel("vzorky")
plt.ylabel("y")
plt.savefig("3_frame_maskon.png")
plt.close()

plt.figure(figsize=(12, 4))
plt.plot(frames_maskoff[52])
plt.title("Ramec bez ruška")
plt.xlabel("vzorky")
plt.ylabel("y")
plt.savefig("3_frame_maskoff.png")
plt.close()

F_maskon = [RATE/(np.argmax(autocorelation_P(center_clipping(frame))[30:])+30) for frame in frames_maskon]
F_maskoff = [RATE/(np.argmax(autocorelation_P(center_clipping(frame))[30:])+30) for frame in frames_maskoff]

print("MASKOFF: stredna hodnota : ",np.mean(F_maskoff), " rozptyl: ",np.var(F_maskoff))
print("MASKON: stredna hodnota : ",np.mean(F_maskon), " rozptyl: ",np.var(F_maskon))

random_frame = frames_maskoff[51]

plt.figure(figsize=(12, 4))
plt.plot(random_frame)
plt.title("Rámec")
plt.xlabel("vzorky")
plt.ylabel("y")
plt.savefig("4_frame.png")
plt.close()

plt.figure(figsize=(12, 4))
plt.plot(center_clipping(random_frame))
plt.title("Rámec po centrálnom klipování")
plt.xlabel("vzorky")
plt.ylabel("y")
plt.savefig("4_center_clipping.png")
plt.close()

plt.figure(figsize=(12, 4))
plt.plot(autocorelation_P(center_clipping(random_frame)))
plt.title("Rámec po autokorelácii")
plt.xlabel("vzorky")
plt.ylabel("y")
plt.savefig("4_autocorrelation.png")
plt.close()

plt.figure(figsize=(12, 4))
plt.plot(F_maskoff,label="maskoff")
plt.plot(F_maskon,label="maskon")
plt.title("Základné frekvencie rámcov")
plt.xlabel("rámec")
plt.ylabel("frekvencia")
plt.legend()
plt.savefig("4_frequency.png")
plt.close()



DFT_maskoff = np.array([ np.fft.fft(frame,1024)[:512] for frame in frames_maskoff ])
DFT_maskon = np.array([ np.fft.fft(frame,1024)[:512] for frame in frames_maskon ])

plt.imshow(np.transpose(10 * np.log10(np.abs(DFT_maskoff)**2)), origin="lower", aspect='auto', extent=[0.0, 1.0, 0, 8000])
plt.title("Spectogram bez rúšky")
plt.xlabel("čas")
plt.ylabel("frekvencia")
plt.colorbar()
plt.savefig("5_spectogram_maskoff.png") 
plt.close()

plt.imshow(np.transpose(10 * np.log10(np.abs(DFT_maskon)**2)), origin="lower", aspect='auto', extent=[0.0, 1.0, 0, 8000])
plt.colorbar()
plt.title("Spectogram s rúškou")
plt.xlabel("čas")
plt.ylabel("frekvencia")
plt.savefig("5_spectogram_maskon.png")
plt.close()

# tu je ten vozrec H(e^jw)
vysledok = DFT_maskon/DFT_maskoff
frekvencna_ch = np.mean(np.abs(vysledok),axis=0)

# plt.plot(8000*np.linspace(0, 1, len(vysledok))), 10 * np.log10(np.abs(vysledok)**2)
# plt.savefig("fooooo.png")
# plt.close()

plt.figure(figsize=(12, 4))
plt.plot(8000*np.linspace(0,1,len(frekvencna_ch)),frekvencna_ch)
plt.title("Frekvenčná charakteristika")
plt.xlabel("frekvencia (Hz)")
plt.ylabel("Spektálna hustota výkonu (dB)")
plt.savefig("6_frekvencna_charakteristika.png")
plt.close()

# PRevod frekvencnej charakteristiky na impulznu odozvu a.k.a naš filter
fil = np.fft.ifft(frekvencna_ch,1024)[:512]

plt.figure(figsize=(12, 4))
plt.plot(fil)
plt.title("Impulzna odozva")
plt.xlabel("vzorky")
plt.ylabel("y")
plt.savefig("7_impulzna_odozva.png")
plt.close()

simulovana_veta = signal.lfilter(fil.real, [1], maskoff_sentence)
simulovany_ton = signal.lfilter(fil.real, [1], maskoff_wave)

plt.figure(figsize=(12, 4))
plt.plot(maskoff_sentence)
plt.title("Veta bez rušky")
plt.xlabel("vzorky")
plt.ylabel("y")
plt.savefig("8_maskoff_sentence.png")
plt.close()

plt.figure(figsize=(12, 4))
plt.plot(simulovana_veta)
plt.title("Simulovana veta")
plt.xlabel("vzorky")
plt.ylabel("y")
plt.savefig("8_sim_sentence.png")
plt.close()

plt.figure(figsize=(12, 4))
plt.plot(maskon_sentence)
plt.title("Veta s ruškou")
plt.xlabel("vzorky")
plt.ylabel("y")
plt.savefig("8_maskon_sentence.png")
plt.close()

write("../audio/sim_maskon_sentence.wav",RATE,np.int16(simulovana_veta))
write("../audio/sim_maskon_tone.wav",RATE,np.int16(simulovany_ton))


# potom si to ulož do filu



# 11 uloha
# https://numpy.org/doc/stable/reference/routines.window.html
# 13 uloha
# ISS room DC tryhards
# 10 tiež na DC wikipedia add overlap
# 15 si mi pomahal myslim , to je ten posun ... je to za 5b taže ez a je tam nejaka slovna bonusova za 2b