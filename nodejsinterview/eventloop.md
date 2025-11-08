event loop is the mechanism that allows nodejs to perform non-blocing I/O operations like reading a file or quering database despite js being single-threaded.
event loop nai euta mechanism jasle garda nodejs le I/O non-blocking(async) operations haru garna sakxa like reading the file or querting the db in single-thread.


the single thread takes the task and insteda of waiting by the stove(block) they immediate pass the order to the kitchen staff(threads/internal worker thereads). the manager then moves on to take the next cusotmer order. when the kitchen is done, they ring a bell and the manager pits the finished mean on the tray(callback queue). the manager constantly checks the queue and picts the next finished task(callback function) to process.

KEY PHASES:
Event loop isn't one simple process, it moves theough several specifiv phases in a cycle:
timers: executres setTimeout() and setInterval() callbacks.
pending callbacks: executres I/O system callbacks that were deferred ot the next loop iternation.
poll : this is the most critical phase, it retreives new I/O events(like a finished db query or file read) and exeutres their related callbacks. if there are no immediate events, it might block here breifly, waiting.
check : executres setImmediate() callbacks.
close callbacks: executres cleanup callbacks like socket.on('close') after an i/o operatoin closes.

Nodejs le long running tasks haru OS wa worker threads lai gar vanera dinxa so that the main js thread is never blocked.

suppose hamile CPU intensive loop chai nodejs process lai diyau vane,
cpu intensive loop asynchronous hudaina, synchronous js code ho.
synchronous code main js thread mai run hunxa jaslai event loop le manage garxa. tesaile arulai assign gardaina.
woker thread haru I/O operations like disk access or networking ko lagi use hune ho, wa, woker threads lai explicitly tyo cpu-bounded kaam garna lagae matra ho.
tesaile jaba pani cpu-intensive task is given to thte nodejs, main thread of event loop is tied up essentily stuck.

main thread nai block vayepaxi, naya request haru handle nai garna sakdiana.
it can't do poll, check , timers etc.
this means the server can't processs new incoming http request, respond to ocmpleted I/O tasks(like a db query that just finishde ) or execute any waiting callbacks. 
it becomes unresponsive, a state often called "event loop blockage" or "starvation"
#### event loop blockage = starvation 

jaba pani performnace related question sodhinxa, teti bela cpu boundend intensive task main thread bata remove garnu parne hunxa vanera explain garnu parne hunxa so that starvation ko scenario occur nai nahos. either by using the built in C++ thread pool for I/O or more realiably by explicitly worker thread lai cpu heavy work ko lagi configure garera.

### Event Execution Order: setImmediate, nextTick and setTime(fn, 0)

Process.nextTick() , setTimeout(fn,0) and setImmediate() iniharu keys ho jasle callback flow manage garna kaam lagxa.

1. Microtask Queue( Highest Priority):
 This queue holds tasks like callbacks from process.nextTick() and resolved promises(.then(), .catch(), .finally())

 proceess.nextTick() immediatly call hunxa jabapani current sycnhronous code finish hunxa arko timeer phase ma janu aghi(timer phase).
 euta process sakera tick lagako jasari tick lagako ho.

            [so microtask queue is made to callbacks jasto task jun chai process.nextTick() bata ako callbacks haru handle garxa. ra aru tasks jastai resolved vako promises bata: .then(), .catch() or .finally() jasto tasks haru hold garxa .  

            process.nextTick() chai synchonous code finish hunasath execute hune ho. ani event loop arko time phase ma janu aghi execute hunxa. process lai one more time tick lagako jasari  tick lagaunxa.]
                        - yo synchronous code ho.

2. Macrotask queue(phase-specific Priority)
yo queue le chai offical event loop phases haruka task haru hold garera rakhxa.

jastai setTimeout(fn,0)L this callback is placed in the timers phases queu. it waits until the timers phase is reached in the next full loop cycle.
setTimeout callback lai: timer phase queue ma rakheko xa. it waits until the next full loop cycle.
setImmediate(): yeslai check phase queue ma rakheko xa. same loop cycle ma run hunxa


pahila suru synchonous code run hune vayo. like console.log()


tespaxi process.nextTick() kina? kinaki synchornous code finish hunasath process.nextTick() execute hunxa.

microtask is vip lane, all promise callback are treated equally.
tespaxi microtask le hold gareko ko anya tasks like : .then() , .catch() or .finally()
macrotask le hold gareko kura: yesma pani 2 ota cateogury xa:

euta tei cycle ma execute hune: setImmediate()
arko next full cycle ma execute hune: setTimeout()ok

so common performnace bottlenecks:
1. CPU Bound Tasks(The Blocker):
 what it is: synchronous heavy compuations, like complex data manipuration , image processing or heavy encryption/hashing.
 impact: starvation

2. Unoptimized I/O (the waiter):
yo kam garne vaneko woker threads le ho. so slow db queries, unhandled connection pooling or poorly optimzied network calls. while these are asynchronous if you have hundereds of slow database queries piling up.
if you have hundered of slow db quirues tyo chai milera callback queue nai full hunxa.

3. memory leaks and garbage collection (the chores)
 what it is L failing to clean up unused memory, leading to memory consumption creep
 impact:node's v8 engine has to spend more and more time running the garbage collector (GC), which is a blocking operation. excessive GC pauses

### solution for performance:
CPU-BOUND TASK: decomponse and isolate. woker thread lai heavy computation kaam haru ma lagaune jasle garda main event loop chai block nahos.
Scaling for traffic: Horizontal scaling: use the cluster module or container orchestotor like cubernetics.
Garbage collection : Monitoring/profiling