/** 
 * File:   Lib_A_RGBF_ring_buffer.c
 * Author: Isaev
 *
 * Created on 8 октября 2017 г., 19:45
 */

//******************************************************************************
//  Секция include: здесь подключается заголовочный файл к модулю
//          Lib_A_RGBF_ring_buffer.h
#include   "Lib_A_RGBF_ring_buffer.h"
//******************************************************************************


//******************************************************************************
//------------------------------------------------------------------------------
//  Глобальные переменные
//------------------------------------------------------------------------------


//------------------------------------------------------------------------------
//  Локальные переменные
//------------------------------------------------------------------------------
//******************************************************************************


//******************************************************************************
//  Секция прототипов локальных функций
//******************************************************************************


//******************************************************************************
//  Секция описания функций (сначала глобальных, потом локальных)
//==============================================================================

/**
 *  @brief  Функция выполняет запись байта информации в кольцевой буфер;
 *  @param  *ringBuff:  Указатель на структуру, содержащую указатели для работы 
 *                      с кольцевым буфером;
 *  @param  *value: Указатель на байты данных, которые необходимо записать в кольцевой буфер;
 *  @param  bytesNumb:  Количество байт, которое необходимо записать в кольцевой буфер
 */
void RGBF_RingBuff_In(RGBF_ring_buf_s *ringBuff,
                      void *pValue,
                      size_t bytesNumb)
{
    size_t i;
    for (i = 0; i < bytesNumb; i++)
    {
        if (ringBuff->pWrite == ringBuff->endBuff) //   Если указатель указывает на 
            //                                          адрес конца буфера;
        {

            *ringBuff->pWrite++ = *(char*) pValue++; // Записываем по указанному адресу 
            //                                          (в конец буфера) данные и инкрементируем 
            //                                          указатель на переменную, которую 
            //                                          необходимо записать в буфер;  
        }
        else if (ringBuff->pWrite > ringBuff->endBuff) //   Если "вылетели" за 
            //                                          пределы массива буфера
        {
            ringBuff->pWrite = ringBuff->startBuff; //  Переносим указатель на адрес начала буфера;  
            *ringBuff->pWrite++ = *(char*) pValue++; // Записываем по указанному адресу 
            //                                          данные и инкрементируем указатель на то, 
            //                                          куда будем записывать данные в 
            //                                          следующей итерации;
        }
        else
        {
            *ringBuff->pWrite++ = *(char*) pValue++; // Записываем по указанному адресу 
            //                                          данные и инкрементируем адрес буфера;
        }
    }
}

/**
 *  @brief  Функция выполяет чтение данных из кольцевого буфера;
 *  @param  *ringBuff:  Указатель на структуру, содержащую указатели для работы 
 *                      с кольцевым буфером;
 *  @param  *pArr:  Указатель на массив данных, куда необходимо скопировать данные 
 *                  из кольцевого буфера;
 *  @param  lenght: Количество байт, которое необходимо скопировать из кольцевого буфера;
 */
void RGBF_RingBuff_Out(RGBF_ring_buf_s *ringBuff,
                       void *pArr,
                       size_t lenght)
{
    size_t i;
    for (i = 0; i < lenght; i++)
    {
        if (ringBuff->pRead == ringBuff->endBuff) //    Если указатель указывает 
            //                                          на адрес конца буфера;
        {
            *(char*) pArr++ = *ringBuff->pRead++; //      Записываем в массив данные из буфера, 
            //                                  находящиеся по указанному адресу и 
            //                                  инкрементируем адрес массива, 
            //                                  в который записали данные;           
        }
        else if (ringBuff->pRead > ringBuff->endBuff) //    Если "вылетели" за 
            //                                              пределы массива буфера
        {
            ringBuff->pRead = ringBuff->startBuff; //   Переносим указатель на адрес начала буфера;
            *(char*) pArr++ = *ringBuff->pRead++; //    Записываем в массив данные из 
            //                                      буфера и инкрементируем указатель 
            //                                      на массив в который записываем 
            //                                      данные и указатель на то, откуда 
            //                                      считали данные буфера;
        }
        else
        {
            *(char*) pArr++ = *ringBuff->pRead++; //    Записываем в массив данные из 
            //                                      буфера и инкрементируем указатель 
            //                                      на массив в который записываем 
            //                                      данные и указатель на то, откуда 
            //                                      считали данные буфера;
        }
    }
}

/**
 *  @brief  Функция проверяет количество байт в кольцевом буфере, которое не было 
 *          скопировано;
 *  @param  *ringBuff:  Указатель на структуру, содержащую указатели для работы 
 *                      с кольцевым буфером;
 *  @return Количество необработаных байт кольцевого буфера;
 */
size_t RGBF_RingBuffCheckDataCnt(RGBF_ring_buf_s *ringBuff)
{
    size_t temp1, temp2;
    if (ringBuff->pWrite >= ringBuff->pRead) // Если указатель на запись больше 
        //                                      чем указатель на чтение;
    {
        return (ringBuff->pWrite - ringBuff->pRead);
    }
    else if (ringBuff->pWrite == ringBuff->pRead) //    Если указатель на запись 
        //  данных в массив совпадает с указателем на чтение данных из массива;
    {
        return 0;
    }
    else // Если указатель на запись меньше чем указатель на чтение (т.е. указатель 
        //  на запись "перешагнул" через конец буфера);
    {
        temp1 = ringBuff->endBuff - ringBuff->pRead;
        temp2 = ringBuff->pRead - ringBuff->pWrite;
        return (temp1 + temp2);
    }
}

/**
 *  @brief  Функция устанавливает указатели чтения и записи кольцевого буфера в 
 *          начало массива кольцевого буфера;
 *  @param  *ringBuff:  Указатель на структуру, содержащую указатели для работы 
 *                      с кольцевым буфером;
 *  @param  *pArr:  Указатель на 0-й элемент массива кольцевого буфера;
 */
void RGBF_ResetRingBuff(RGBF_ring_buf_s *ringBuff)
{
    ringBuff->pRead = ringBuff->startBuff;
    ringBuff->pWrite = ringBuff->startBuff;
}
//==============================================================================
//******************************************************************************


////////////////////////////////////////////////////////////////////////////////
//  END OF FILE
////////////////////////////////////////////////////////////////////////////////
