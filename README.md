# USB-Hi-Res-Audio-USB-I2S-STM32F446RC
USB I2S преобразователь 32bit/96kHz, <a class="link" href="https://www.chipdip.ru/product0/9000569733">SUPER PRIME chipdip</a>
Этот конвертер USB / I2S с максимальной частотой дискретизации 192 кГц и максимальным разрешением 32 бита. Все возможные конфигурации представлены в таблице 1. Он обеспечивает воспроизведение Hi-Res аудио студийного качества в двух цифровых каналах I2S, которые с помощью цифроаналоговых преобразователей могут быть представлены в форматах:<br/>
2.0 – стерео, (stereo) <br/>
3.1 – объемное звучание с басовым каналом,<br/>
4.0 – квадро. quadro<br/>
<h2>Назначение разъемов и кнопок</h2>

<p><a class="galery" href="https://static.chipdip.ru/lib/367/DOC004367901.jpg"><img alt="Схема" src="https://static.chipdip.ru/lib/367/DOC004367901.jpg" /></a></p>


С помощью джамперов на плате выбираются четыре основных аппаратных режима работы (таблица 2), а программные режимы выбираются в настройках аудио операционной системы.
<h2>Возможные конфигурации. Таблица 1</h2>

<table>
	<tbody>
		<tr>
			<td rowspan="2">Конфигурация<br />
			аудио каналов</td>
			<td rowspan="2">Разрешение<br />
			bit</td>
			<td colspan="4">Частота дискретизации kHz</td>
		</tr>
		<tr>
			<td>44,1</td>
			<td>48</td>
			<td>96</td>
			<td>192</td>
		</tr>
		<tr>
			<td rowspan="3">2.0 Stereo</td>
			<td>16</td>
			<td>+</td>
			<td>+</td>
			<td>+</td>
			<td>+</td>
		</tr>
		<tr>
			<td>24</td>
			<td>+</td>
			<td>+</td>
			<td>+</td>
			<td>&nbsp;</td>
		</tr>
		<tr>
			<td>32</td>
			<td>&nbsp;</td>
			<td>&nbsp;</td>
			<td>+</td>
			<td>&nbsp;</td>
		</tr>
		<tr>
			<td rowspan="2">3.1</td>
			<td>16</td>
			<td>+</td>
			<td>+</td>
			<td>+</td>
			<td>&nbsp;</td>
		</tr>
		<tr>
			<td>24</td>
			<td>+</td>
			<td>+</td>
			<td>&nbsp;</td>
			<td>&nbsp;</td>
		</tr>
		<tr>
			<td rowspan="2">4.0 quadro</td>
			<td>16</td>
			<td>+</td>
			<td>+</td>
			<td>+</td>
			<td>&nbsp;</td>
		</tr>
		<tr>
			<td>24</td>
			<td>+</td>
			<td>+</td>
			<td>&nbsp;</td>
			<td>&nbsp;</td>
		</tr>
	</tbody>
</table>
<h2>Аппаратная конфигурация. Таблица 2</h2>

<table>
	<tbody>
		<tr>
			<td>Позиция джампера</td>
			<td>Аудио режим</td>
			<td>Битность</td>
		</tr>
		<tr>
			<td>Не установлены</td>
			<td>Стерео 2.0</td>
			<td>16, 24</td>
		</tr>
		<tr>
			<td>14</td>
			<td>Стерео 2.0</td>
			<td>32</td>
		</tr>
		<tr>
			<td>13</td>
			<td>3.1</td>
			<td>16</td>
		</tr>
		<tr>
			<td>13,14</td>
			<td>Квадро 4.0</td>
			<td>16</td>
		</tr>
	</tbody>
</table>

<h2>Схема</h2>

<p><a class="galery" href="https://static.chipdip.ru/lib/367/DOC004367363.jpg"><img alt="Схема" src="https://static.chipdip.ru/lib/367/DOC004367362.jpg" /></a><br />
<br />
<br />
SUPER PRIME chipdip&nbsp;прекрасно работает со всеми&nbsp;существующими модулями цифровой обработки звука, всеми&nbsp;цифро-аналоговыми преобразователями,&nbsp;построенными в лаборатории Чип и Дип. И непременно будет испытан со всеми цифровыми аудио устройствами которые мы спроектируем в будущем.</p>

<h2>Пример подключения четырех каналов к <a class="link" href="/product/pcm5102a-audio-dac">PCM5102</a></h2>
<img alt="" src="https://static.chipdip.ru/lib/368/DOC004368189.jpg" />
<h2>Пример подключения четырех каналов к ADAU1701 <a class="link" href="/product/digital-signal-processors-rdc2-0027v1-2">RDC2-0027v1</a></h2>
<br />
<img alt="" src="https://static.chipdip.ru/lib/368/DOC004368408.jpg" />
<h2>Пример подключения четырех каналов к ADAU1701 <a class="link" href="/product/dsp-rdc2-0027v2-2">RDC2-0027v2</a></h2>
<img alt="" src="https://static.chipdip.ru/lib/368/DOC004368433.jpg" />
<h2>Пример подключения четырех каналов к ADAU1701 <a class="link" href="/product/rdc2-0027v3">RDC2-0027v3</a></h2>
<br />
<img alt="" src="https://static.chipdip.ru/lib/368/DOC004368191.jpg" />
<h2>Пример подключения&nbsp;двух каналов к <a class="link" href="/product/sigmadsp-adau1761">ADAU1761</a></h2>
<img alt="" src="https://static.chipdip.ru/lib/368/DOC004368434.jpg" /><br />
<em><strong>Изменения в новой версии прошивки v1.1:</strong></em><br />
- для конфигураций 3.1 и 4.0 добавлены форматы<br />
&nbsp; 24 бита, 44100 Гц<br />
&nbsp; 24 бита, 48000 Гц<br />
- реализована функция &quot;Mute&quot; (при управлении с ПК)<br />
<br />
<img alt="" src="https://static.chipdip.ru/lib/369/DOC004369719.jpg" /><br />
&nbsp;
<strong><em>Изменения в новой версии прошивки v1.2</strong><br />
- реализована внешняя синхронизация<br />
- незначительные исправления<br />
<br />
<strong>Назначение контактов. прошивка v1.2</strong><br />
<strong>контакт 2</strong> - вход для внешнего MCLK<br />
<strong><strong>контакты 11 и 12</strong></strong> - выбор внешнего тактирования<br />
разомкнуты - внутреннее тактирование<br />
замкнуты - внешнее тактирование<br />
<br />
<strong>контактны 8 и 9</strong> - выходы переключения частот<br />
&nbsp;
<table>
	<tbody>
		<tr>
			<td>Контакт 8</td>
			<td>Контакт 9</td>
			<td>частота</td>
		</tr>
		<tr>
			<td>0</td>
			<td>0</td>
			<td>44100</td>
		</tr>
		<tr>
			<td>0</td>
			<td>1</td>
			<td>48000</td>
		</tr>
		<tr>
			<td>1</td>
			<td>0</td>
			<td>96000</td>
		</tr>
		<tr>
			<td>1</td>
			<td>1</td>
			<td>192000</td>
		</tr>
	</tbody>
</table>
<br />
<img alt="" src="https://static.chipdip.ru/lib/422/DOC004422648.jpg" /><br />
<br />
<strong><em>Изменения в новой версии прошивки v1.3:</em></strong><br />
Реализована функция генератора MCLK. Пока устройство не подключено к ПК (при внешнем питании 5В), оно генерирует сигнал MCLK с выбранной частотой. Возможна как внутренняя, так и внешняя синхронизация. После подключения устройства к USB частота MCLK определяется аудиопотоком.
<table>
	<tbody>
		<tr>
			<td>Позиция джампера</td>
			<td>Частота<br />
			MCLK (256 * Fs), МГц</td>
		</tr>
		<tr>
			<td>не установлены</td>
			<td>11,2896</td>
		</tr>
		<tr>
			<td>6-7</td>
			<td>12,288</td>
		</tr>
		<tr>
			<td>4-5</td>
			<td>24,576</td>
		</tr>
		<tr>
			<td>4-5 и 6-7</td>
			<td>49,152</td>
		</tr>
	</tbody>
</table>
<br />
<br />

<br>
<a class="link" href="https://www.chipdip.ru/catalog/just-do-it">all projects</a>
