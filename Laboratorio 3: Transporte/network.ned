simple Generator
{
    parameters:
        volatile double generationInterval; // sec
        int packetByteSize;
        @display("i=block/source");
    gates:
        output out;
}

simple Queue
{
    parameters:
        volatile int bufferSize;
        volatile double serviceTime; // sec
        @display("i=block/queue;q=buffer");
    gates:
        input in;
        output out;
}

simple Sink
{
    parameters:
        @display("i=block/sink");
    gates:
        input in;
        //output out; //Nuevo
}

simple TransportTx
{
    parameters:
        int bufferSize;
        @display("i=block/queue;q=buffer");
    gates:
        input toApp;
        inout toOut;
}

simple TransportRx
{
    parameters:
        int bufferSize;
        @display("i=block/queue;q=buffer");
    gates:
        output toApp;
        inout toOut;
}


module NodeTx
{
    parameters:
        @display("i=block/routing");
    gates:
        inout inOut;
    submodules:
        gen: Generator {
            parameters:
                @display("p=75,50");
        }
        //We changed the queue for the new module TransportTx
        traTx: TransportTx {
            parameters:
                @display("p=75,190");
        }
    connections:
        gen.out --> traTx.toApp;
        inOut <--> traTx.toOut;
}

module NodeRx
{
    parameters:
        @display("i=block/routing");
    gates:
        inout inOut;
    submodules:
        sink: Sink {
            parameters:
                @display("p=75,50");
        }
        //We changed the queue for the new module TransportRx
        traRx: TransportRx {
            parameters:
                @display("p=75,190");
        }
    connections:
        inOut <--> traRx.toOut;
        //Caso  problema de flujo
        traRx.toApp --> {datarate = 0.5Mbps;} --> sink.in;
        //Caso problema congestión
        //traRx.toApp --> {datarate = 1Mbps;} --> sink.in;
}

network Network
{
    @display("bgl=2;bgb=261,156");
    submodules:
        nodeTx: NodeTx {
            @display("p=30,69");
        }
        queue0: Queue {
            @display("p=130,30");
        }
        queue1: Queue {//Nueva cola de feedback
            @display("p=130,119");
        }
        nodeRx: NodeRx {
            @display("p=231,69");
        }
    connections:
        nodeTx.inOut$o --> {  datarate = 1Mbps; delay = 10us; } --> queue0.in;
        //Caso problema de flujo
        queue0.out --> {  datarate = 1Mbps; delay = 10us; } --> nodeRx.inOut$i;
        //Caso problema de congestión
        //queue0.out --> {  datarate = 0.5Mbps; delay = 10us; } --> nodeRx.inOut$i;
        //
        nodeRx.inOut$o --> {  datarate = 1Mbps; delay = 10us; } --> queue1.in;
        queue1.out --> {  datarate = 1Mbps; delay = 10us; } --> nodeTx.inOut$i;
}
