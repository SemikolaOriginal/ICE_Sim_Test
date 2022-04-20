# ICE_Sim_Test
Симуляция и тестирование двигателя внутреннего сгорания (ДВС) на перегрев по заданному графику зависимости крутящего момента от скорости вращения коленвала. 
>**Примечание 0.1:** Полагается, что двигатель работает без нагрузки, а весь вырабатываемый момент идет на раскрутку коленвала. 


>**Примечание 0.2:** Отсутствует логика старта двигателя (симуляция стартера), двигатель начинает работу с заданного нулевого отсчета зависимости крутящего момента от скорости вращения коленвала.

>**Примечание 0.3:** Температура двигателя незадолго до момента старта равна температуре окружающей среды.

>**Примечание 0.4:** Нагрев и охлаждение действуют на двигатель постоянно, одновременно и независимо друг от друга.

## 1 Описание двигателя
Двигатель задается следующим набором параметров:

 - Момент инерции двигателя ![formula](https://render.githubusercontent.com/render/math?math=I (kg*m^{2}))
 - Кусочно-линейная зависимость крутящего момента ![formula](https://render.githubusercontent.com/render/math?math=M), вырабатываемого
   двигателем, от скорости вращения коленвала ![formula](https://render.githubusercontent.com/render/math?math=V) (крутящий момент в ![formula](https://render.githubusercontent.com/render/math?math=N*m),
   скорость вращения в ![formula](https://render.githubusercontent.com/render/math?math=\frac{rad}{s}))
 - Температура перегрева ![formula](https://render.githubusercontent.com/render/math?math=T_{o} (C^{0}))     
 - Коэффициент зависимости скорости нагрева от крутящего момента ![formula](https://render.githubusercontent.com/render/math?math=H_M (\frac{C^{0}} {N*m*s}))
 - Коэффициент зависимости скорости нагрева от скорости вращения коленвала ![formula](https://render.githubusercontent.com/render/math?math=H_V (\frac{C^{0}*s}{rad^{2}}))
 - Коэффициент зависимости скорости охлаждения от температуры двигателя и окружающей среды ![formula](https://render.githubusercontent.com/render/math?math=C (\frac{1}{s}))
Данные параметры задаются в конфигурационном файле **engine.cfg** построчно в указанном выше порядке, либо в параметризованном указанными параметрами конструкторе.
> **Примечание 1.1:** функция ![formula](https://render.githubusercontent.com/render/math?math=M(V)) задается в файле построчно, сначала множество значений ![formula](https://render.githubusercontent.com/render/math?math=M), разделенные запятой, затем множество значений ![formula](https://render.githubusercontent.com/render/math?math=V), разделенные запятой.


> **Примечание 1.2:** для параметров ![formula](https://render.githubusercontent.com/render/math?math=I), ![formula](https://render.githubusercontent.com/render/math?math=M) , ![formula](https://render.githubusercontent.com/render/math?math=V) значения задаются из множества неотрицательных чисел, для ![formula](https://render.githubusercontent.com/render/math?math=T_{o}) значения задаются из множества целых чисел, для ![formula](https://render.githubusercontent.com/render/math?math=H_M), ![formula](https://render.githubusercontent.com/render/math?math=H_V), ![formula](https://render.githubusercontent.com/render/math?math=C) значения задаются из множества неотрицательных рациональных чисел.

Конфигурационная база двигателя задается в базовом классе `EngineConfig`, описание класса лежит в файле `EngineConfig.h`, описание методов класса произведено в модуле `EngineConfig.cpp`.
Создать объект класса `EngineConfig` можно, используя следующие конструкторы:

		EngineConfig(
			unsigned int set_I, 
			map<unsigned int, 
			unsigned int set_MV, 
			int set_To, 
			double set_HM, 
			double set_HV, 
			double set_C)
		EngineConfig(string config_path)

Проверка корректности параметров двигателя задается используемыми типами данных и методами класса `EngineConfig`

		void checkDataCorrects();
		virtual bool isWrong();

> **Примечание 1.3:** Конструктор класса `EngineConfig` содержит проверку `checkDataCorrects` по отрицательным результатам которой происходит уничтожение объекта.

## 2 Симуляция двигателя
Симуляция работы двигателя происходит на основе заданного описания двигателя. Двигатель задается классом `Engine`, наследующим класс `EngineCofig`.
##### Для работы с двигателем предусмотрены методы:

		void start(int ambientTemperature);
		void stop();
		void update(double deltaTime, int ambientTemperature);
		void cooling(int setEngineTemperature);
	
##### Для проверки состояния двигателя предусмотрены методы:

		bool is_started();
		bool is_overheat();
		bool is_MaxCrankshaftSpeedReached();

##### Для вычисления параметров двигателя предусмотрены методы:

		int getCurrentM();
		double getCurrentHeatCoef();
		double getCurrentCoolingCoef(int ambientTemperature);
		double getCurrentEngineTempIncrement(double time, int ambientTemperature);
		int getCurrentTemperature();
		double getDeltaTime();

Основой симуляции работы двигателя является метод `update` , который симулирует работу двигателя за промежуток времени `deltaTime`. В методе производится:
 - расчет приращение скорости вращения коленвала ![formula](https://render.githubusercontent.com/render/math?math=V) - `deltaV`;
 - расчет приращения температуры двигателя `temp` для `Engine.currentTemperature`;
 - определение состояния перегрева `Engine.isOverheat`.

> **Примечание 2.1:** скорость нагрева двигателя рассчитывается как ![formula](https://render.githubusercontent.com/render/math?math=V_H=M*H_M+V^2*H_V[\frac{C^0}{s}]).


> **Примечание 2.2:** скорость охлаждения двигателя рассчитывается как ![formula](https://render.githubusercontent.com/render/math?math=V_C=C*(T_{ambient}-T_{engine}) [\frac{C^0}{s}]).

## 3 Тестирование двигателя
При тестировании двигателя симулируется его старт, работа через метод `Engine.update` до момент обнаружения перегрева в цикле симуляции его работы и останов двигателя. 
Между тестами желательно охладить двигатель методом `Engine.cooling`.

> **Примечание 3.1:** для расчета симуляции используется модельное время.


Тестирование двигателя реализовано в модуле `Tests.cpp`. Для запроса температуры окружающей среды на старте работы двигателя используется функция:

		int readAmbientTemperature();

Для тестирования реализованы функции:

		void overheatEngineLinerTimeTest(shared_ptr<Engine>& engine, int ambientTemperature);
		void overheatEngineAutoTimeTest(shared_ptr<Engine>& engine, int ambientTemperature);

### 3.1 Расчет методом симуляции
Основой для расчета методом симуляции работы ДВС является зависимость крутящего момента от скорости вращения коленвала ![formula](https://render.githubusercontent.com/render/math?math=M(V)). Расчет производится методом односторонних прямоугольников для численного интегрирования по ![formula](https://render.githubusercontent.com/render/math?math=dV) - задается в коде константно.
Данная зависимость не отражает время работы двигателя прямо. Для вычисления времени работы двигателя на участке применены два подхода:

 1. полагается, что время работы двигателя на одном интервале ![formula](https://render.githubusercontent.com/render/math?math=deltaV) зависимости ![formula](https://render.githubusercontent.com/render/math?math=M(V)) константно и коленвал раскручивается линейно и независимо от вырабатываемого крутящего момента ![formula](https://render.githubusercontent.com/render/math?math=M);
 2. полагается, что время работы двигателя на одном куске зависимости ![formula](https://render.githubusercontent.com/render/math?math=M(V)) зависит от вырабатываемого крутящего момента.

В первом случае ![formula](https://render.githubusercontent.com/render/math?math=deltaTime) задается константно и каждая итерация цикла симуляции изменяет время ![formula](https://render.githubusercontent.com/render/math?math=time) линейно, и расчет приращения температуры двигателя происходит на равных интервалах времени.
	
Во втором случае ![formula](https://render.githubusercontent.com/render/math?math=deltaTime) задается функциональной зависимостью ![formula](https://render.githubusercontent.com/render/math?math=\alpha(M)=\frac{M}{I}.) Данная зависимость позволяет оценить время пребывания двигателя на интервале ![formula](https://render.githubusercontent.com/render/math?math=deltaV) как ![formula](https://render.githubusercontent.com/render/math?math=\Delta) ![formula](https://render.githubusercontent.com/render/math?math=t=\frac{I}{M}*\Delta) ![formula](https://render.githubusercontent.com/render/math?math=V), полагая ![formula](https://render.githubusercontent.com/render/math?math=\frac{\Delta_M}{\Delta_V}) близким к нулю. Учитывая, что при расчете используется метод односторонних прямоугольников с левой точкой, то данный расчет дает верхнюю/нижнюю оценку времени при возрастании/убывании ![formula](https://render.githubusercontent.com/render/math?math=M(V)) соответственно.
