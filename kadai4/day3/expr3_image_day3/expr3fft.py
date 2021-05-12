"""
情報工学実験3：課題4　第3回 フーリエ変換
Copyright(C) 2019 Tokyo University of Science, Department of Information and Computer Technology
"""
import warnings; warnings.filterwarnings('ignore')
import numpy as np
from skimage.io import imread, imsave
from skimage.transform import resize, rotate
from skimage.color import rgb2gray
from numpy.fft import fft2, ifft2, fftshift, ifftshift
import matplotlib.pyplot as plt
plt.gray()

def do_fft(im):
    """ フーリエ変換 """
    f = fft2(im, axes=(0,1))       # フーリエ変換
    fshift = fftshift(f)           # 原点が配列の中心になるようにシフト
    return fshift

def do_ifft(fshift):
    """ 逆フーリエ変換 """
    f = ifftshift(fshift)           # 原点が配列の先頭になるようにシフト
    im = ifft2(f, axes=(0,1)).real  # フーリエ逆変換
    return im

def power_spectrum(fshift):
    """ フーリエ変換のパワースペクトルを計算 """
    foo = np.abs(fshift)
    foo = np.where(foo < 1, 1, foo)
    return 20 * np.log10(foo)    # パワースペクトルの算出

def ffilter(filter_func, shape):
    """ 周波数フィルタの計算 """
    height, width = shape[:2]
    iscale = 1 / width
    jscale = 1 / height
    foo = [filter_func(i * iscale, j * jscale) 
        for j in range(0 - height//2, height - height//2)
            for i in range(0 - width//2, width - width//2)]
    if len(shape) == 2:
        # グレースケール画像
        return np.array(foo).reshape(height, width)
    else:
        # カラー画像
        return np.array(foo).reshape(height, width, 1)

def array_normalize(ar):
    """ 配列の正規化 定数倍して値を(0, 1)の範囲に収める"""
    armin = ar.min()
    armax = ar.max()
    return (ar - armin)/(armax - armin + 1.0e-12)

def draw_src_image(im):
    """ 入力画像の描画（左部）"""
    plt.subplot(1, 3, 1).set_title('Original Image')
    plt.imshow(im)

def draw_power_spectrum(fshift):
    """ パワースペクトルの描画（中央部）"""
    ax = plt.subplot(1, 3, 2)
    plt.cla()
    ax.set_title('Power Spectrum')
    #imshow(array_normalize(power_spectrum(fshift)))
    plt.imshow(power_spectrum(fshift).astype(np.uint8))

# 右半分の領域
def draw_ifft_image(fshift):
    """ 逆フーリエ変換の描画（右部）"""
    plt.subplot(1, 3, 3).set_title('Inverse Fourier Transform')
    im = do_ifft(fshift)
    plt.imshow(array_normalize(im))

def draw_all(im, fshift):
    """ 周波数フィルタリングの描画 """
    plt.figure(figsize=(12, 8))
    draw_src_image(im)
    draw_power_spectrum(fshift)
    draw_ifft_image(fshift)

def draw_src_power_spectrum(im, fshift):
    """ 周波数フィルタリングの描画 """
    plt.figure(figsize=(12, 8))
    draw_src_image(im)
    draw_power_spectrum(fshift)
    
def do_ffilter(fshift, func):
    """ 周波数フィルタリング """
    return fshift * ffilter(func, fshift.shape)

def show_hybrid_image(im):
    """ ハイブリッド画像の表示 """
    nscale = 5
    margin = 2
    height, width = im.shape[:2]
    
    # 出力画像サイズを算出
    outim_width = 0
    for i in range(nscale):
        outim_width += width//(i+1) + margin
    outim_width -= margin
    
    # 出力画像を格納する配列を作成
    if im.ndim == 2:
        # グレースケール画像
        outim = np.zeros((height, outim_width))
    else:
        # カラー画像
        outim = np.zeros((height, outim_width, 3))
    
    # 出力画像を作成
    xpos = 0
    va = "top"
    for i in range(nscale):
        sh, sw = height//(i+1), width//(i+1)
        small_im = resize(im, (sh, sw))
        outim[height-sh:, xpos:xpos+sw] = small_im
        plt.text(xpos, height-sh, "Scale=%d"%(i+1), color="yellow", va=va, ha="left", fontsize=12)
        va = "bottom"
        xpos += sw + margin
    plt.imshow(outim)
    plt.axis("off")
    
