# Mechatronics Project - Plant Monitor

## Arduino Code

This section of code writtern by myself forms a part of a larger Mechatronics group project. It is only for the Arduino Mega 2560 and is to be run alongside separate code from the ESP32 which is where the local LLM is hooked up with.  

This code runs by itself and its purpose is to display real-time data on an OLED display and to log data for 24 hours to an external SD card where the data can then be given to the LLM model to analysis the data.

## Purpose of the System

Environmental regulation plays a paramount role in determining plant physiological performance, with parameters such
as temperature, relative humidity, soil moisture, and atmospheric composition exerting strong influence over metabolic
activity and growth dynamics. Sustained deviations or transient perturbations in these variables can
propagate non-linear effects within controlled environments, necessitating continuous, high-resolution monitoring and
structured analysis rather than intermittent manual observation. Within this context, the system functions as a
decision-support monitoring platform that characterises environmental dynamics and provides short-term predictive
insight rather than direct environmental control.

The system described implements a multi-parameter environmental monitoring and analysis platform designed to
characterise environmental behaviour over extended temporal windows while supporting real-time observation and
historical analysis. A dual-layer computational architecture is employed to separate deterministic sensing and data
acquisition from higher-level analytical processing. Environmental measurements are acquired using an Arduino Mega
microcontroller, which interfaces directly with heterogeneous sensors and performs time-stamped logging of all
environmental variables to an SD card in CSV (Excel-compatible) format. This layer prioritises temporal consistency,
reliability, and reproducibility of the acquired dataset. All sensing, data logging, analytical processing, forecasting, and
conversational interaction are executed locally within the system and do not require network connectivity, ensuring
deterministic behaviour and robustness in deployment environments where external infrastructure may be unavailable.
Higher-order analysis is performed on an ESP32 processing layer, which operates on historical sensor data to extract
statistical and temporal descriptors of environmental behaviour. Recorded time series are transformed into a structured
feature space comprising measures of central tendency, temporal variability, anomaly density, and rates of change.
These features are aggregated into an environmental state matrix, providing a compact system-level representation of
environmental stability and dominant sources of fluctuation across a 24-hour monitoring period. This representation
enables comparative assessment of environmental variables that is not readily achievable through inspection of raw
time-series data alone.

Temporal intelligence is further derived through trend extraction, rolling variability analysis, and anomaly detection
applied to each environmental signal. Short-term forecasting is implemented using recent temporal features to generate
predictive estimates over a three-hour horizon, supporting anticipatory assessment of environmental behaviour.
Forecast outputs, generated independently of the conversational interface, are intended to characterise near-term system
evolution rather than long-range prediction and are used to contextualise detected anomalies and emerging trends.
To facilitate interrogation of recorded environmental information, a locally hosted large language model (LLM) is
integrated as a human–machine interaction layer. The LLM operates over aggregated 24-hour summary statistics
derived from the logged sensor data, enabling structured natural-language interrogation of overall environmental
conditions, parameter ranges, and general system behaviour. In addition, the LLM functions as a general-purpose
conversational and computational interface, supporting numerical calculations, logical reasoning, explanatory queries,
and open-ended information requests. User input is provided via a terminal-based interface, with generated responses
rendered concurrently in the terminal environment and on a dedicated TFT display. These conversational capabilities
are intentionally decoupled from the environmental analysis and forecasting pipelines, preserving the determinism and
integrity of the sensing and analytical subsystems.

Collectively, the system functions as an integrated environmental sensing and intelligence platform that extends
conventional monitoring through structured feature extraction, temporal analysis, and data-driven interpretation. By
combining deterministic embedded sensing with computational analysis and a modular interaction layer, the system
enables a level of environmental characterisation that supports informed decision-making in controlled growth
environments and provides a scalable framework for intelligent monitoring applications while adhering to low-cost
design constraints.
