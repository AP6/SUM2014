/* FILENAME: IMAGE.C
 * PROGRAMMER: AP6
 * PURPOSE: Image handle functions
 * LAST UPDATE: 06.06.2014
 */

#include "image.h"

/* Функция загрузки изображения.
 * АРГУМЕНТЫ:
 *   - указатель на обрабатываемую картинку:
 *       IMAGE *Img;
 *   - имя загружаемого файла:
 *       HDC hDC;
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ:
 *   (BOOL) результат загрузки (TRUE - успешно).
 */
BOOL ImageLoad( IMAGE *Img, CHAR *FileName )
{
  HBITMAP hBmLoad;

  if (Img == NULL)
    return FALSE;
  Img->W = Img->H = 0;
  Img->hBm = NULL;
  Img->hDC = NULL;
  Img->Bits = NULL;
  if ((hBmLoad = LoadImage(NULL, FileName,
         IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE)) != NULL)
  {
    BITMAP bm;
    BITMAPINFOHEADER bmi;
    HDC hDC, hMemDC;

    GetObject(hBmLoad, sizeof(bm), &bm);
    Img->W = bm.bmWidth;
    Img->H = bm.bmHeight;

    bmi.biSize = sizeof(BITMAPINFOHEADER);
    bmi.biWidth = Img->W;
    bmi.biHeight = -Img->H;
    bmi.biBitCount = 32;
    bmi.biPlanes = 1;
    bmi.biCompression = BI_RGB;
    bmi.biSizeImage = Img->W * Img->H * 4;
    bmi.biXPelsPerMeter = bmi.biYPelsPerMeter = 1;
    bmi.biClrUsed = 0;
    bmi.biClrImportant = 0;

    Img->hBm = CreateDIBSection(NULL, (BITMAPINFO *)&bmi, DIB_RGB_COLORS,
      (VOID **)&Img->Bits, NULL, 0);

    hDC = GetDC(NULL);
    hMemDC = CreateCompatibleDC(hDC);
    Img->hDC = CreateCompatibleDC(hDC);

    SelectObject(hMemDC, hBmLoad);
    SelectObject(Img->hDC, Img->hBm);
    BitBlt(Img->hDC, 0, 0, Img->W, Img->H, hMemDC, 0, 0, SRCCOPY);

    DeleteDC(hMemDC);
    ReleaseDC(NULL, hDC);
    DeleteObject(hBmLoad);
  }

  return Img->hBm != NULL;
} /* End of 'ImageLoad' function */

/* Функция освобождения памяти из-под изображения.
 * АРГУМЕНТЫ:
 *   - указатель на обрабатываемую картинку:
 *       IMAGE *Img;
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ: Нет.
 */
VOID ImageFree( IMAGE *Img )
{
  if (Img == NULL)
    return;
  if (Img->hBm != NULL)
    DeleteObject(Img->hBm);
  DeleteDC(Img->hDC);
  Img->W = Img->H = 0;
  Img->hBm = NULL;
  Img->hDC = NULL;
  Img->Bits = NULL;
} /* End of 'ImageFree' function */

/* Функция получения цвета точки изображения.
 * АРГУМЕНТЫ:
 *   - указатель на обрабатываемую картинку:
 *       IMAGE *Img;
 *   - координаты получаемой точки:
 *       INT X, Y;
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ:
 *   (DWORD) цвет получаемой точки.
 */
DWORD ImageGetP( IMAGE *Img, INT X, INT Y )
{
  if (Img == NULL)
    return 0;
  if (Img->hBm != NULL &&
      X >= 0 && Y >= 0 && X < Img->W && Y < Img->H)
  {
    DWORD color;
    BYTE r, g, b;

    color = Img->Bits[Y * Img->W + X];
    b = color & 0xFF;
    g = (color >> 8) & 0xFF;
    r = (color >> 16) & 0xFF;

    return RGB(r, g, b);
  }
  return 0;
} /* End of 'ImageGetP' function */


/* Функция рисования изображения.
 * АРГУМЕНТЫ:
 *   - указатель на обрабатываемую картинку:
 *       IMAGE *Img;
 *   -контекст усройства отображения:
 *       HDC hDC;
 *   - координаты вывода:
 *       INT X, Y;
 *   -код логической операции:
 *       INT LogOp;
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ:
 *   (DWORD) цвет получаемой точки.
 */
VOID ImageDraw( IMAGE *Img, HDC hDC, INT X, INT Y, INT LogOp )
{
  if (Img == NULL)
    return;
  BitBlt(hDC, X, Y, Img->W, Img->H, Img->hDC, 0, 0, LogOp);
}


/* END OF 'IMAGE.C' FILE */
